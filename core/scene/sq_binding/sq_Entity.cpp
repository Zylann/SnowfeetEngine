#include "../Entity.hpp"
#include "sq_scene.hpp"
#include "../../squirrel/bind_tools.hpp"
#include <memory>

namespace sn
{

// This wrapper ensures the returned value to be of the 
static SQInteger Entity_sqGetChildByName(HSQUIRRELVM v)
{
    if (sq_gettop(v) == 2)
    {
        Sqrat::Var<std::shared_ptr<Entity> > self(v, 1);
        Sqrat::Var<const std::string&> childName(v, 2);
        if (!Sqrat::Error::Occurred(v))
        {
            Entity::Ref child = self.value->getChildByName(childName.value);
            if (child)
                child->pushSquirrelObject(v);
            else
                sq_pushnull(v);
            return 1;
        }
        return sq_throwerror(v, Sqrat::Error::Message(v).c_str());
    }
    return sq_throwerror(v, _SC("wrong number of parameters (expected 1)"));
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
    c.SquirrelFunc("getChildByName", Entity_sqGetChildByName);
        
    RootTable(vm).Bind(className, c);
}
    
} // namespace sn
