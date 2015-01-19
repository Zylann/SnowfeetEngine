#include "../Entity.hpp"
#include "sq_scene.hpp"
#include "../../squirrel/sqrat.h"
#include <memory>

namespace sn
{

typedef std::shared_ptr<Entity> EntityRef;

static const std::string &  Entity_getName(EntityRef e) { return e->getName(); }
static void                 Entity_setName(EntityRef e, const char * s) { return e->setName(s); }
static bool                 Entity_hasTag(EntityRef e, const char * s) { return e->hasTag(s); }
static void                 Entity_addTag(EntityRef e, const char * s) { return e->addTag(s); }
static void                 Entity_removeTag(EntityRef e, const char * s) { return e->removeTag(s); }
//static EntityRef            Entity_createChild(EntityRef e) { return e->createChild(); }
static void                 Entity_destroyChildren(EntityRef e) { return e->destroyChildren(); }

void registerEntity(HSQUIRRELVM vm)
{
    using namespace Sqrat;

    Class<EntityRef> c(vm);

    c.GlobalProp("name", Entity_getName, Entity_setName);
    c.GlobalFunc("hasTag", Entity_hasTag);
    c.GlobalFunc("addTag", Entity_addTag);
    c.GlobalFunc("removeTag", Entity_removeTag);

    RootTable(vm).Bind("Entity", c);
}

} // namespace sn



