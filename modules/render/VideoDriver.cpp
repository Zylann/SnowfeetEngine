#include "VideoDriver.h"
#include "Texture.h"
#include <GL/glew.h>
#include "gl_check.h"

namespace sn
{

    namespace
    {
        //------------------------------------------------------------------------------
        GLenum genericPrimitiveTypeToGL(MeshPrimitiveType pt)
        {
            switch (pt)
            {
            case SN_MESH_POINTS: return GL_POINTS;
            case SN_MESH_LINES: return GL_LINES;
            case SN_MESH_TRIANGLES: return GL_TRIANGLES;
            default:
                SN_WARNING("Invalid state");
                return GL_LINES;
            }
        }
    }

//------------------------------------------------------------------------------
VideoDriver::VideoDriver() : m_context(nullptr)
{
    // TODO Make it configurable
    GLContextSettings contextSettings;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;
    m_context = new GLContext(contextSettings);
    m_context->makeCurrent();
}

//------------------------------------------------------------------------------
VideoDriver::~VideoDriver()
{
    if (m_context)
        delete m_context;
}

//------------------------------------------------------------------------------
sn::Texture * VideoDriver::createTexture()
{
    return new Texture();
}

//------------------------------------------------------------------------------
void VideoDriver::makeCurrent(bool isCurrent)
{
    m_context->makeCurrent(isCurrent);
}

//------------------------------------------------------------------------------
void VideoDriver::makeCurrent(RenderScreen & screen)
{
    screen.makeCurrent(*m_context);
}

//------------------------------------------------------------------------------
void VideoDriver::useProgram(const ShaderProgram * shader)
{
    if (shader)
    {
        // Start using shader
        glCheck(glUseProgram(shader->getID()));
    }
    else
    {
        // Stop using shader
        glCheck(glUseProgram(0));
    }
}

//------------------------------------------------------------------------------
void VideoDriver::clearTarget(ClearMask mask)
{
    u32 gl_mask = 0;
    if (mask & SNR_CLEAR_COLOR)
        gl_mask |= GL_COLOR_BUFFER_BIT;
    if (mask & SNR_CLEAR_DEPTH)
        gl_mask |= GL_DEPTH_BUFFER_BIT;
    glClear(gl_mask);
}

//------------------------------------------------------------------------------
void VideoDriver::clearColor(const Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

//------------------------------------------------------------------------------
void VideoDriver::setDepthTest(bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------------------
void VideoDriver::setViewport(u32 x, u32 y, u32 width, u32 height)
{
    glViewport(x, y, width, height);
}

//------------------------------------------------------------------------------
void VideoDriver::setViewport(const IntRect & rect)
{
    setViewport(rect.x(), rect.y(), rect.width(), rect.height());
}

//------------------------------------------------------------------------------
void VideoDriver::drawMesh(const Mesh & mesh)
{
    if (mesh.isEmpty())
        return;

    // TODO this is immediate draw, data is sent everytime.
    // We should support VBO too.

    // TODO Refactor Mesh data description so we can avoid lots of copy/paste and hardcodings

    // Positions
    glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &mesh.getVertices()[0]));
    glCheck(glEnableVertexAttribArray(0));

    // Colors
    if (!mesh.getColors().empty())
    {
        glCheck(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, &mesh.getColors()[0]));
        glCheck(glEnableVertexAttribArray(1));
    }

    GLenum primitiveType = genericPrimitiveTypeToGL(mesh.getInternalPrimitiveType());

    // UV
    if (!mesh.getUV().empty())
    {
        glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, &mesh.getUV()[0]));
        glCheck(glEnableVertexAttribArray(2));
    }

    // Normals
    if (!mesh.getNormals().empty())
    {
        glCheck(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, &mesh.getNormals()[0]));
        glCheck(glEnableVertexAttribArray(3));
    }

    std::vector<u32> floatBufferIDs;
    auto & floatBuffers = mesh.getCustomFloats();
    if (!floatBuffers.empty())
    {
        u32 floatBufferIDStart = 5;
        for (u32 i = 0; i < floatBuffers.size(); ++i)
        {
            auto & floatBuffer = floatBuffers[i];
            if (!floatBuffer.empty())
            {
                u32 id = floatBufferIDStart + i;
                glCheck(glVertexAttribPointer(id, 1, GL_FLOAT, GL_FALSE, 0, &floatBuffer[0]));
                glCheck(glEnableVertexAttribArray(id));
                floatBufferIDs.push_back(id);
            }
        }
    }

    std::vector<u32> vec2BufferIDs;
    auto & vec2Buffers = mesh.getCustomVec2Buffers();
    if (!vec2Buffers.empty())
    {
        u32 vec2BufferIDStart = 10;
        for (u32 i = 0; i < vec2Buffers.size(); ++i)
        {
            auto & vec2Buffer = vec2Buffers[i];
            if (!vec2Buffer.empty())
            {
                u32 id = vec2BufferIDStart + i;
                glCheck(glVertexAttribPointer(id, 2, GL_FLOAT, GL_FALSE, 0, &vec2Buffer[0]));
                glCheck(glEnableVertexAttribArray(id));
                vec2BufferIDs.push_back(id);
            }
        }
    }

    if (mesh.getIndices().empty())
    {
        // Draw without indices
        glCheck(glDrawArrays(
            primitiveType,
            0, mesh.getVertices().size()
        ));
    }
    else
    {
        // Draw with indices
        glCheck(glDrawElements(
            primitiveType,
            mesh.getIndices().size(),
            GL_UNSIGNED_INT,
            &mesh.getIndices()[0]
        ));
    }

    for (auto it = vec2BufferIDs.begin(); it != vec2BufferIDs.end(); ++it)
        glCheck(glDisableVertexAttribArray(*it));

    for (auto it = floatBufferIDs.begin(); it != floatBufferIDs.end(); ++it)
        glCheck(glDisableVertexAttribArray(*it));

    if (!mesh.getNormals().empty())
        glCheck(glDisableVertexAttribArray(3));

    if (!mesh.getUV().empty())
        glCheck(glDisableVertexAttribArray(2));

    if (!mesh.getColors().empty())
        glCheck(glDisableVertexAttribArray(1));

    glCheck(glDisableVertexAttribArray(0));
}

/// \brief Clamps next draw calls to a sub-rectangle on the current render target.
//------------------------------------------------------------------------------
void VideoDriver::setScissor(IntRect rect)
{
    glCheck(glScissor(rect.x(), rect.y(), rect.width(), rect.height()));
    glCheck(glEnable(GL_SCISSOR_TEST));
}

/// \brief Disables scissor.
//------------------------------------------------------------------------------
void VideoDriver::disableScissor()
{
    glCheck(glDisable(GL_SCISSOR_TEST));
}

//------------------------------------------------------------------------------
void VideoDriver::setBlendMode(BlendMode mode)
{
    if (mode != SNR_BLEND_NONE)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}


} // namespace sn

