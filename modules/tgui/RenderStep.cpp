#include <core/util/typecheck.h>
#include <core/scene/Scene.h>

#include "RenderStep.h"
#include "GUI.h"

using namespace sn;

namespace tgui
{

void RenderStep::onDraw(sn::render::VideoDriver & driver)
{
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

