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

namespace priv
{
	/// \deprecated
    template<class T>
    void destroyClassInstance_checkRefCounted(std::true_type, T * p)
    {
        // If the class derives from RefCounted,
        // use its release() method instead of delete (which is protected)
        p->release();
    }

	/// \deprecated
    template<class T>
    void destroyClassInstance_checkRefCounted(std::false_type, T * p)
    {
        // If it's a regular class, just delete it
        delete p;
    }

	/// \deprecated
    template<class T>
    void destroyClassInstance(T * p)
    {
        priv::destroyClassInstance_checkRefCounted<T>(std::is_base_of<RefCounted, T>(), p);
    }
}

template <typename T>
SQInteger releaseClassInstance(SQUserPointer ptr, SQInteger size)
{
    T * p = static_cast<T*>(ptr);
    priv::destroyClassInstance(p);
    return 0;
}

template <typename T>
SQInteger createClassInstance(HSQUIRRELVM vm)
{
    T * p = new T();
    sq_setinstanceup(vm, -1, p);
    sq_setreleasehook(vm, -1, releaseClassInstance<T>);
    return 0;
}

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
// Forward-declare binding functions
namespace test_squirrel
{
    void registerSomething(HSQUIRRELVM vm);
    void registerSharedThing(HSQUIRRELVM vm);
    void registerDerivedThing(HSQUIRRELVM vm);
}

//------------------------------------------------------------------------------
namespace squirrel
{
    template <> inline SQUserPointer getTypeTag<Something>() { return test_squirrel::registerSomething; }
    template <> inline SQUserPointer getTypeTag<SharedThing>() { return test_squirrel::registerSharedThing; }
    template <> inline SQUserPointer getTypeTag<DerivedThing>() { return test_squirrel::registerDerivedThing; }
}

//------------------------------------------------------------------------------

// Binding

namespace
{
    //--------------------------------------------------------------------------
    // Raw binding of a C++ class

    int Something_doStuff(HSQUIRRELVM vm)
    {
        Something * self = nullptr;
        if (squirrel::getNativeInstance(vm, 1, self))
        {
            self->doStuff();
        }
        return 0;
    }

	int Something_getText(HSQUIRRELVM vm)
	{
        Something * self = nullptr;
        if (squirrel::getNativeInstance(vm, 1, self))
        {
		    sq_pushstring(vm, self->getText().c_str(), -1);
		    return 1;
        }
        return 0;
	}

	int Something_setText(HSQUIRRELVM vm)
	{
        Something * self = nullptr;
        if (squirrel::getNativeInstance(vm, 1, self))
        {
            const char * str = squirrel::getString(vm, 2);
		    if (str)
			    self->setText(str);
        }
		return 0;
	}

    //--------------------------------------------------------------------------
    // Binding of a ScriptableObject 

    int SharedThing_sayHello(HSQUIRRELVM vm)
    {
        SharedThing * self = nullptr;
        if (squirrel::getNativeInstance(vm, 1, self))
        {
            self->sayHello();
        }
        return 0;
    }

    int SharedThing_getChild(HSQUIRRELVM vm)
    {
        SharedThing * self = nullptr;
        if (squirrel::getNativeInstance(vm, 1, self))
        {
            SharedThing * sub = self->getChild();
            sub->pushScriptObject(vm);
            return 1;
        }
        return 0;
    }

    //--------------------------------------------------------------------------
    // Binding of a DerivedThing

    int DerivedThing_sayHello(HSQUIRRELVM vm)
    {
        DerivedThing * self = nullptr;
        if (squirrel::getNativeInstance(vm, 1, self))
        {
            self->sayHello();
        }
        return 0;
    }
}

namespace test_squirrel
{
    void registerSomething(HSQUIRRELVM vm)
    {
        const char * className = "Something";

        squirrel::Class c(vm, className);
        c.setConstructor(createClassInstance<Something>);
        c.setMethod("doStuff", Something_doStuff);
		c.setMethod("setText", Something_setText);
		c.setMethod("getText", Something_getText);

        squirrel::RootTable(vm).setObject(className, c);
    }

    void registerSharedThing(HSQUIRRELVM vm)
    {
        ObjectTypeDatabase::get().registerType<SharedThing>();

        ScriptableObject::bindBase<SharedThing>(vm)
            .setMethod("sayHello", SharedThing_sayHello)
            .setMethod("getChild", SharedThing_getChild);
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
    test_squirrel::registerSomething(vm);
    test_squirrel::registerSharedThing(vm);
    test_squirrel::registerDerivedThing(vm);

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

