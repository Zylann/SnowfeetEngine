#include <core/util/typecheck.h>
#include <core/scene/Scene.h>

#include "RenderStep.h"
#include "GUI.h"

namespace tgui
{

SN_OBJECT_IMPL(tgui::RenderStep)

void RenderStep::onDraw(sn::VideoDriver & driver)
{
    using namespace sn;

    Scene * scene = getScene();
    if (scene)
    {
        // TODO GUIs need a manager or a driver
        std::vector<GUI*> guis;
        scene->getChildrenOfType<GUI>(guis);
        for (auto it = guis.begin(); it != guis.end(); ++it)
        {
            GUI * gui = *it;
            gui->draw(driver);
        }
    }
}

} // namespace tgui

