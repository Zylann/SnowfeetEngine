#include "sq_scene.hpp"
#include <iostream>

//namespace Sqrat {
//        template<>                                                            
//    struct Var<sn::Entity*> {
//        
//        sn::Entity* value;
//        Var(HSQUIRRELVM vm, SQInteger idx) : 
//            value(ClassType<sn::Entity>::GetInstance(vm, idx, true))
//        {}                                                                
//        static void push(HSQUIRRELVM vm, sn::Entity* value) {
//                value->pushSquirrelObject(vm);                                
//                /*ClassType<_className>::PushInstance(vm, value);*/           
//                sq_setreleasehook(
//                    vm, -1, &sqRefCountedReleaseHook<sn::Entity>
//                );                                                            
//        }                                                                 
//    };                                                                    
//} /* namespace Sqrat */

namespace sn
{

// This wrapper ensures the returned value to be of the 
static SQInteger Entity_sqGetChildByName(HSQUIRRELVM v)
{
    if (sq_gettop(v) == 2)
    {
        Sqrat::Var<Entity*> self(v, 1);
        Sqrat::Var<const std::string&> childName(v, 2);
        if (!Sqrat::Error::Occurred(v))
        {
            Entity * child = self.value->getChildByName(childName.value);
            if (child)
            {
                child->pushSquirrelObject(v);
                sq_setreleasehook(v, -1, &Sqrat::sqRefCountedReleaseHook<sn::Entity>);
            }
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
        
//    RootTable(vm).Bind(className, Class<Entity>(vm, className)
    RootTable(vm).Bind(className, Class<Entity, RefCountedAllocator<Entity>>(vm, className)
        .Prop("name", &Entity::getName, &Entity::setName)
        .Func("hasTag", &Entity::hasTag)
        .Func("addTag", &Entity::addTag)
        .Func("removeTag", &Entity::removeTag)
        .Func("destroyChildren", &Entity::destroyChildren)
        .Func("createChild", &Entity::createChildNoParams)
        .Func("setUpdatable", &Entity::setUpdatable)
        .SquirrelFunc("getChildByName", &Entity_sqGetChildByName)
    );
    
    //c.SquirrelFunc("getChildByName", Entity_sqGetChildByName);
}
    
} // namespace sn


