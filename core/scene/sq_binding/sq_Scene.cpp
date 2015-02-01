#include "sq_scene.hpp"

namespace sn
{

void registerScene(HSQUIRRELVM vm)
{
    // TODO FIXME The following doesn't compiles yet...

    using namespace Sqrat;

    const char * className = "Scene";

    DerivedClass<Scene,Entity,RefCountedAllocator<Scene>> c(vm, className);

    c.Func("quit", &Scene::quit);

    RootTable(vm).Bind(className, c);
}

} // namespace sn

