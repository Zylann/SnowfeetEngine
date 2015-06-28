#include "../scene/Drawable.hpp"

namespace sn
{

void bindDrawable(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Drawable>(vm);
}

} // namespace sn

