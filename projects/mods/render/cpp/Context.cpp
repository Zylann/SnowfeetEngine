#include "Context.hpp"
#include <GL/glew.h>

namespace sn {
namespace render {

Context::Context(Window & owner, ContextSettings settings) :
    m_impl(nullptr),
    r_window(&owner),
    m_settings(settings)
{
    initImpl(owner);
}

Context::~Context()
{
    deinitImpl();
}

void Context::clearTarget()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Context::drawMesh(const Mesh & mesh)
{
    if (mesh.isEmpty())
        return;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &mesh.getVertices()[0]);
    glEnableVertexAttribArray(0);

    glDrawArrays(mesh.getInternalPrimitiveType(), 0, mesh.getVertices().size());

    glDisableVertexAttribArray(0);
}

void Context::useProgram(const ShaderProgram * shader)
{
    if (shader)
    {
        // Start using shader
        glUseProgram(shader->getID());
    }
    else
    {
        // Stop using shader
        glUseProgram(0);
    }
}

} // namespace render
} // namespace sn

