#include "../entities/DrawableBase.h"

namespace sn {
namespace render {

void bindDrawableBase(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Drawable>(vm);
}

} // namespace render
} // namespace sn
