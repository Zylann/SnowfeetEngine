#include "../entities/Drawable.h"

namespace sn {
namespace render {

void bindDrawable(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Drawable>(vm);
}

} // namespace render
} // namespace sn
