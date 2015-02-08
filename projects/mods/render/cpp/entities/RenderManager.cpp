#include "RenderManager.hpp"
#include <core/system/gui/SystemGUI.hpp>
#include <core/scene/Scene.hpp>

namespace sn {
namespace render {

const char * RenderManager::TAG = "RenderManager";

RenderManager::RenderManager() : Entity(), m_context(nullptr)
{
    // This entity is always present across scenes
    setFlag(SN_EF_STICKY, true);
}

RenderManager::~RenderManager()
{
    if (m_context)
        delete m_context;
}

void RenderManager::onReady()
{
    SN_ASSERT(m_context == nullptr, "Invalid state, m_context is not null");

    // TODO If two renderManager are created, just use another window ID and it might work
    SN_ASSERT(getScene()->getTaggedEntity(TAG) == nullptr, "Two RenderManagers is not supported at the moment");

    addTag(TAG);

    // Register to update manager.
    // Arbitrary values. The layer is given quite high so gameplay should be updated before.
    setUpdatable(true, 0, 32000);

    // Get or create window in which we'll render
    Window * win = SystemGUI::get().getWindowByID(0);
    if (win == nullptr)
        win = SystemGUI::get().createWindow();

    // Create rendering context
    if (win)
        m_context = new Context(*win);
}

void RenderManager::onUpdate()
{
    // Render!
    render();
}

void RenderManager::render()
{
    if (m_context == nullptr)
        return; // Can't render

    m_context->clearTarget();

    // BEGIN TEST CODE

    Mesh mesh;
    //mesh.setPrimitiveType(SNR_PT_TRIANGLES);
    mesh.addPosition(-0.5f, -0.5f);
    mesh.addPosition(0.f, 0.5f);
    mesh.addPosition(0.5f, -0.5f);

    m_context->drawMesh(mesh);

    // END TEST CODE

    // Display rendered surface
    m_context->swapBuffers();
}

} // namespace render
} // namespace sn

