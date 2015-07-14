#include "tests.hpp"

#include <core/reflect/Object.h>

using namespace sn;

// Testing class
class ReflectedClass : public Object
{
public:
    SN_OBJECT(ReflectedClass, sn::Object)

    ReflectedClass():
        m_counter(42)
    {
        SN_LOG("Created " << getObjectType().getName() << " instance");
    }

    const std::string & getName()
    {
        return m_name;
    }

    void setName(const std::string & name)
    {
        SN_LOG("Name set to '" << name << "'");
        m_name = name;
    }

    s32 getCounter() { return m_counter; }

private:
    std::string m_name;
    s32 m_counter;

};

void test_reflection()
{
    //s32(ReflectedClass::*method)();
    //std::function<void(Object*,void*)> getterClosure = [=](Object* obj, void* data){
    //    *(s32*)data = (((ReflectedClass*)obj)->*method)();
    //};

    auto & otb = ObjectTypeDatabase::get();

    // Register

    ObjectType & ot = otb.registerType<ReflectedClass>();
    //ot.addProperty("name", &ReflectedClass::getName, &ReflectedClass::setName);
    //ot.addProperty("counter", &ReflectedClass::getCounter);

    // Use

    Object * obj = ot.instantiate();
    
    //const ObjectProperty & property = *ot.getProperty("counter");
    //if (!property.is<bool>())
    //    SN_LOG("Hmm, property " << property.getName() << " is not a boolean.");
    //s32 counter = ot.getProperty("counter")->getValue<s32>(obj);
    //SN_LOG("Counter value is " << counter);

    //ot.getProperty("name")->setValue(obj, std::string("It works!"));

    // Cleanup

    delete obj;

}



