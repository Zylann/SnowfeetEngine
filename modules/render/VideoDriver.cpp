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

        GLenum genericTypeToGL(VertexAttribute::Type t)
        {
            switch (t)
            {
            case VertexAttribute::TYPE_INT8: return GL_BYTE;
            case VertexAttribute::TYPE_INT16: return GL_SHORT;
            case VertexAttribute::TYPE_INT32: return GL_INT;

            case VertexAttribute::TYPE_UINT8: return GL_UNSIGNED_BYTE;
            case VertexAttribute::TYPE_UINT16: return GL_UNSIGNED_SHORT;
            case VertexAttribute::TYPE_UINT32: return GL_UNSIGNED_INT;

            case VertexAttribute::TYPE_FLOAT32: return GL_FLOAT;
            case VertexAttribute::TYPE_FLOAT64: return GL_DOUBLE;

            default: SN_ASSERT(false, "Invalid state"); return 0;
            }
        }
    }


//------------------------------------------------------------------------------
SN_OBJECT_IMPL(VideoDriver)

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
    // Get attributes
    const VertexDescription & vertexFormat = mesh.getVertexDescription();
    const VertexAttributeList & attributeList = vertexFormat.getAttributes();

    // Bind vertex arrays
    for (auto it = attributeList.begin(); it != attributeList.end(); ++it)
    {
        const VertexAttribute & attrib = *it;
        const Mesh::VertexArray & va = mesh.getVertexArray(attrib.use);

        if (!va.data.empty())
        {
            glCheck(glVertexAttribPointer(attrib.use, attrib.count, genericTypeToGL(attrib.type), GL_FALSE, 0, va.data.data()));
            glCheck(glEnableVertexAttribArray(attrib.use));
        }
    }

    GLenum primitiveType = genericPrimitiveTypeToGL(mesh.getInternalPrimitiveType());

    // TODO Draw-calls statistic?
    // Draw vertices
    if (mesh.getIndices().empty())
    {
        // Draw without indices
        u32 vcount = mesh.getVertexCount();
        glCheck(glDrawArrays(primitiveType, 0, vcount));
    }
    else
    {
        // Draw with indices
        const std::vector<u32> & indices = mesh.getIndices();
        glCheck(glDrawElements(
            primitiveType,
            indices.size(),
            GL_UNSIGNED_INT,
            indices.data()
        ));
    }

    // Unbind vertex arrays
    for (auto it = attributeList.begin(); it != attributeList.end(); ++it)
    {
        const VertexAttribute & attrib = *it;
        const Mesh::VertexArray & data = mesh.getVertexArray(attrib.use);

        if (!data.data.empty())
        {
            glCheck(glDisableVertexAttribArray(attrib.use));
        }
    }
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

