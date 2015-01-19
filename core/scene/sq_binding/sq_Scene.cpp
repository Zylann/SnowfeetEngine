#include "../Scene.hpp"
#include "sq_scene.hpp"
#include "../../squirrel/sqrat.h"

namespace sn
{

void registerScene(HSQUIRRELVM vm)
{
    using namespace Sqrat;

    RootTable(vm).Bind("Scene", Class<Scene>(vm));
}

} // namespace sn


