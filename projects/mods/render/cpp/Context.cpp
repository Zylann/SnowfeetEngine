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
    glClear(GL_COLOR_BUFFER_BIT);
}

//------------------------------------------------------------------------------
void Context::drawMesh(const Mesh & mesh)
{
    if (mesh.isEmpty())
        return;

    // Positions
    glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &mesh.getVertices()[0]));
    glCheck(glEnableVertexAttribArray(0));

    // Colors
    if (!mesh.getColors().empty())
    {
        glCheck(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, &mesh.getColors()[0]));
        glCheck(glEnableVertexAttribArray(1));
    }

    glCheck(glDrawArrays(mesh.getInternalPrimitiveType(), 0, mesh.getVertices().size()));

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

} // namespace render
} // namespace sn

