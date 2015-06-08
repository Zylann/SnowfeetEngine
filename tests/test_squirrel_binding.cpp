#include "tests.hpp"
#include <iostream>
#include <core/squirrel/ScriptVM.h>
#include <core/squirrel/ScriptClass.h>
#include <core/squirrel/ScriptTable.h>
#include <core/util/stringutils.hpp>

using namespace sn;

//------------------------------------------------------------------------------
// Helpers (should be included in the engine)
namespace
{
    template <typename T>
    SQInteger createClassInstance(HSQUIRRELVM vm)
    {
        T * p = new T();
        sq_setinstanceup(vm, -1, p);
        sq_setreleasehook(vm, -1, deleteClassInstance<T>);
        return 0;
    }

    template <typename T>
    SQInteger deleteClassInstance(SQUserPointer ptr, SQInteger size)
    {
        T * p = static_cast<T*>(ptr);
        delete p;
        return 0;
    }

    template <typename T>
    T * checkClassInstance(HSQUIRRELVM vm, int i)
    {
        SQUserPointer p = NULL;
        sq_getinstanceup(vm, i, &p, NULL);
        return static_cast<T*>(p);
    }
}

void debugStackDump(HSQUIRRELVM vm) { sn::debugStackDump(vm); }

//------------------------------------------------------------------------------
// The native class
class Something
{
public:
    Something()
    {
        std::cout << "Something constructed" << std::endl;
    }

    ~Something()
    {
        std::cout << "Something destroyed" << std::endl;
    }

    void doStuff()
    {
        std::cout << "Hello I am something!" << std::endl;
    }
};

//------------------------------------------------------------------------------
// Binding
namespace
{
    int Something_doStuff(HSQUIRRELVM vm)
    {
        auto * self = checkClassInstance<Something>(vm, 1);
        self->doStuff();
        return 0;
    }

    void registerSomething(HSQUIRRELVM vm)
    {
        const char * className = "Something";

        ScriptClass c(vm, className);
        c.setConstructor(createClassInstance<Something>);
        c.setMethod("doStuff", Something_doStuff);

        ScriptRootTable(vm).setObject(className, c);
    }
}

//------------------------------------------------------------------------------
// C++ execution
void test_squirrelBinding()
{
    // Create the VM
    ScriptVM vmObject;
    HSQUIRRELVM vm = vmObject.getSquirrelVM();

    // Register native interface
    registerSomething(vm);

    {
        // Execute a script

        std::string source;
        if (!readFile("test_data/script1.nut", source))
            return;

        Script script(vm);
        if (!script.compileString(source))
            return;

        script.execute();
    }

    // End
    std::cout << "Finished" << std::endl;
}

