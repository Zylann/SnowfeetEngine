#include "tests.hpp"
#include <iostream>
#include <core/squirrel/VM.h>
#include <core/squirrel/Class.h>
#include <core/squirrel/Table.h>
#include <core/app/ScriptableObject.h>
#include <core/util/stringutils.h>
#include <core/object_types.h>

using namespace sn;

void debugStackDump(HSQUIRRELVM vm) { squirrel::debugStackDump(vm); }

//------------------------------------------------------------------------------
// The native classes

class Something
{
public:
    Something()
    {
        SN_LOG("Something constructed");
		m_text = "(empty)";
    }

    ~Something()
    {
        SN_LOG("Something destroyed");
    }

    void doStuff()
    {
        SN_LOG("Hello I am something, and my text is ");
    }

	const std::string & getText() { return m_text; }
	void setText(const std::string & str) { m_text = str; }

private:
	std::string m_text;
};

//------------------------------------------------------------------------------
class SharedThing : public ScriptableObject
{
public:
    SN_SCRIPT_OBJECT(SharedThing, sn::ScriptableObject)

    SharedThing(const char * name = "(unnamed)", bool createChild=true) : ScriptableObject(),
        m_name(name)
    {
        SN_LOG("SharedThing " << m_name << " constructed");
        m_child = createChild ? new SharedThing("sub-thing", false) : nullptr;
    }

    virtual void sayHello()
    {
        SN_LOG("Hello, I am a thing (" << m_name << ")");
    }

    SharedThing * getChild()
    {
        return m_child;
    }

protected:
    ~SharedThing()
    {
        SN_LOG("SharedThing " << m_name << " destroyed");
        if (m_child)
            m_child->release();
    }

    std::string m_name;

private:
    SharedThing * m_child;
};

//------------------------------------------------------------------------------
class DerivedThing : public SharedThing
{
public:
    SN_SCRIPT_OBJECT(DerivedThing, SharedThing)

    DerivedThing(const char * name = "(unnamed derived)") : SharedThing(name, false)
    {
        SN_LOG("Derived thing constructed");
    }

    void sayHello()
    {
        SN_LOG("Hello, I am a derived thing (" << m_name << ")");
    }

protected:
    ~DerivedThing()
    {
        SN_LOG("Derived thing destroyed");
    }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// Binding

namespace
{
    //--------------------------------------------------------------------------
    // Raw binding of a C++ class

    int Something_doStuff(HSQUIRRELVM vm)
    {
        auto * self = squirrel::getNativeInstance<Something>(vm, 1);
        self->doStuff();
        return 0;
    }

	int Something_getText(HSQUIRRELVM vm)
	{
		auto * self = squirrel::getNativeInstance<Something>(vm, 1);
		sq_pushstring(vm, self->getText().c_str(), -1);
		return 1;
	}

	int Something_setText(HSQUIRRELVM vm)
	{
		auto * self = squirrel::getNativeInstance<Something>(vm, 1);
        const char * str = squirrel::getString(vm, 2);
		if (str)
			self->setText(str);
		return 0;
	}

    void registerSomething(HSQUIRRELVM vm)
    {
        const char * className = "Something";

        squirrel::Class c(vm, className);
        c.setConstructor(squirrel::createClassInstance<Something>);
        c.setMethod("doStuff", Something_doStuff);
		c.setMethod("setText", Something_setText);
		c.setMethod("getText", Something_getText);

        squirrel::RootTable(vm).setObject(className, c);
    }

    //--------------------------------------------------------------------------
    // Binding of a ScriptableObject 

    int SharedThing_sayHello(HSQUIRRELVM vm)
    {
        auto * self = squirrel::getNativeInstance<SharedThing>(vm, 1);
        self->sayHello();
        return 0;
    }

    int SharedThing_getChild(HSQUIRRELVM vm)
    {
        auto * self = squirrel::getNativeInstance<SharedThing>(vm, 1);
        SharedThing * sub = self->getChild();
        sub->pushScriptObject(vm);
        return 1;
    }

    void registerSharedThing(HSQUIRRELVM vm)
    {
        ObjectTypeDatabase::get().registerType<SharedThing>();

        ScriptableObject::bindBase<SharedThing>(vm)
            .setMethod("sayHello", SharedThing_sayHello)
            .setMethod("getChild", SharedThing_getChild);
    }

    //--------------------------------------------------------------------------
    // Binding of a DerivedThing

    int DerivedThing_sayHello(HSQUIRRELVM vm)
    {
        auto * self = squirrel::getNativeInstance<DerivedThing>(vm, 1);
        self->sayHello();
        return 0;
    }

    void registerDerivedThing(HSQUIRRELVM vm)
    {
        ObjectTypeDatabase::get().registerType<DerivedThing>();
        
        ScriptableObject::bindBase<DerivedThing>(vm)
            .setMethod("sayHello", DerivedThing_sayHello);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// C++ execution

void test_squirrelBinding()
{
    // Register engine's types to be sure things work
    sn::registerObjectTypes(ObjectTypeDatabase::get());

    // Create the VM
    squirrel::VM vmObject;
    HSQUIRRELVM vm = vmObject.getSquirrelVM();

    // Register native interface
    registerSomething(vm);
    registerSharedThing(vm);
    registerDerivedThing(vm);

    {
        // Execute a script

        std::string source;
        if (!readFile("test_data/script1.nut", source))
            return;

        squirrel::Script script(vm);
        if (!script.compileString(source))
            return;

        script.execute();
    }

    // End
    std::cout << "Finished" << std::endl;
}

