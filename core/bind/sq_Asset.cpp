#include <core/asset/Asset.h>
#include "sq_core.h"
#include "../squirrel/bind_macros.h"

using namespace squirrel;

namespace sn
{
    //namespace
    //{
    //    CURRENT_CLASS(Asset)
    //    
    //}


void bindAsset(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Asset>(vm);
}

} // namespace sn


