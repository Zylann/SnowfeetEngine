#include "gl_check.hpp"
#include <GL/glew.h>
#include "Context.hpp"

namespace sn {
namespace render {

// TODO Create a main OpenGL context that will be shared will all others
// (A context must be created for each system window).

//namespace
//{
//	Context * g_sharedContext = nullptr;
//    u32 g_contextCount = 0;
//}

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

//------------------------------------------------------------------------------
Context::Context(ContextSettings settings, Window * window, Context * sharedContext):
    m_impl(nullptr),
    r_window(window),
    m_settings(settings)
{
	SN_LOG("Creating render context " << this << ": " << settings.toString() << " (window: " << window << ")");

    //++g_contextCount;
    //if (g_contextCount == 1)
    //{
    //    SN_ASSERT(g_sharedContext == nullptr, "Invalid state: global context is already initialized");
    //    g_sharedContext = new Context(settings);
    //}

    initImpl(sharedContext);
}

//------------------------------------------------------------------------------
Context::~Context()
{
    deinitImpl();
    SN_LOG("Destroying context " << this << " (window: " << (r_window) << ")");
    //--g_contextCount;
    //if (g_contextCount == 0)
    //{
    //    SN_ASSERT(g_sharedContext != nullptr, "Invalid state: global context is not initialized");
    //    delete g_sharedContext;
    //}
}

//------------------------------------------------------------------------------
void Context::clearTarget(ClearMask mask)
{
    u32 gl_mask = 0;
    if (mask & SNR_CLEAR_COLOR)
        gl_mask |= GL_COLOR_BUFFER_BIT;
    if (mask & SNR_CLEAR_DEPTH)
        gl_mask |= GL_DEPTH_BUFFER_BIT;
    glClear(gl_mask);
}

//------------------------------------------------------------------------------
void Context::clearColor(const Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

//------------------------------------------------------------------------------
void Context::setDepthTest(bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------------------
void Context::drawMesh(const Mesh & mesh)
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

//------------------------------------------------------------------------------
void Context::useProgram(const ShaderProgram * shader)
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
void Context::setViewport(u32 x, u32 y, u32 width, u32 height)
{
    glViewport(x, y, width, height);
}

//------------------------------------------------------------------------------
void Context::setViewport(const IntRect & rect)
{
    setViewport(rect.x(), rect.y(), rect.width(), rect.height());
}

} // namespace render
} // namespace sn

