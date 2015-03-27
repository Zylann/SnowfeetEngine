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
//}

//------------------------------------------------------------------------------
Context::Context(Window & owner, ContextSettings settings) :
    m_impl(nullptr),
    r_window(&owner),
    m_settings(settings)
{
	SN_LOG("Creating render context: " << settings.toString());
    initImpl();
}

//------------------------------------------------------------------------------
Context::~Context()
{
    deinitImpl();
}

//------------------------------------------------------------------------------
void Context::clearTarget()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    // Positions
    glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &mesh.getVertices()[0]));
    glCheck(glEnableVertexAttribArray(0));

    // Colors
    if (!mesh.getColors().empty())
    {
        glCheck(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, &mesh.getColors()[0]));
        glCheck(glEnableVertexAttribArray(1));
    }

    // UV
    if (!mesh.getUV().empty())
    {
        glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, &mesh.getUV()[0]));
        glCheck(glEnableVertexAttribArray(2));
    }

    if (mesh.getIndices().empty())
    {
        // Draw without indices
        glCheck(glDrawArrays(
            mesh.getInternalPrimitiveType(), 
            0, mesh.getVertices().size()
        ));
    }
    else
    {
        // Draw with indices
        glCheck(glDrawElements(
            mesh.getInternalPrimitiveType(), 
            mesh.getIndices().size(),
            GL_UNSIGNED_INT, 
            &mesh.getIndices()[0]
        ));
    }

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

