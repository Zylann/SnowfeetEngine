#include "../Entity.hpp"
#include "sq_scene.hpp"
#include "../../squirrel/bind_tools.hpp"
#include <memory>

namespace sn
{

//static const std::string &  Entity_getName(EntityRef e) { return e->getName(); }
//static void                 Entity_setName(EntityRef e, const char * s) { return e->setName(s); }
//static bool                 Entity_hasTag(EntityRef e, const char * s) { return e->hasTag(s); }
//static void                 Entity_addTag(EntityRef e, const char * s) { return e->addTag(s); }
//static void                 Entity_removeTag(EntityRef e, const char * s) { return e->removeTag(s); }
//static EntityRef            Entity_createChild(EntityRef e) { return e->createChild(); }
//static void                 Entity_destroyChildren(EntityRef e) { return e->destroyChildren(); }

static Entity::Ref Entity_createChild(Entity::Ref e, const std::string & typeName)
{
    
}

void registerEntity(HSQUIRRELVM vm)
{
    using namespace Sqrat;

    const char * className = "Entity";

    Class<Entity::Ref> c(vm, className);

    c.Prop("name", &Entity::getName, &Entity::setName);
    c.Func("hasTag", &Entity::hasTag);
    c.Func("addTag", &Entity::addTag);
    c.Func("removeTag", &Entity::removeTag);
    c.Func("destroyChildren", &Entity::destroyChildren);
    c.Func("createChild", &Entity::createChild);
    c.Func("setUpdatable", &Entity::setUpdatable);

    RootTable(vm).Bind(className, c);
}

} // namespace sn



