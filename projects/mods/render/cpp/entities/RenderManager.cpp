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

    Window * win = SystemGUI::get().getWindowByID(0);
    if (win == nullptr)
        win = SystemGUI::get().createWindow();

    //m_context = new Context(*win);
}

} // namespace render
} // namespace sn

