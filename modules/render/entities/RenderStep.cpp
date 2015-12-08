#include "RenderStep.h"

namespace sn {
namespace render {

RenderStep::RenderStep() : sn::Entity(),
    r_manager(nullptr)
{}

RenderManager * RenderStep::getManager() const
{
    if (r_manager == nullptr)
    {
        sn::Entity * parent = getParent();
        while (!parent->isInstanceOf<RenderManager>())
        {
            parent = parent->getParent();
            if (parent == nullptr)
            {
                SN_ERROR("RenderStep " << toString() << " is not child of a RenderManager");
                return nullptr;
            }
        }
        r_manager = checked_cast<RenderManager*>(parent);
    }
    return r_manager;
}

} // namespace render
} // namespace sn

