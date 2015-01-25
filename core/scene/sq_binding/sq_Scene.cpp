#include "../Scene.hpp"
#include "sq_scene.hpp"
#include "../../squirrel/sqrat.h"

namespace sn
{

void registerScene(HSQUIRRELVM vm)
{
    using namespace Sqrat;

    const char * className = "Scene";

    Class<Scene::Ref> c(vm, className);

    RootTable(vm).Bind(className, c);
}

} // namespace sn


