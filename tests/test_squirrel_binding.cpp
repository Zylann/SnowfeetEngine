#include "tests.hpp"
#include <iostream>
#include <core/squirrel/ScriptVM.h>
#include <core/squirrel/ScriptClass.h>
#include <core/squirrel/ScriptTable.h>
#include <core/util/stringutils.hpp>

using namespace sn;

void debugStackDump(HSQUIRRELVM vm) { sn::debugStackDump(vm); }

//------------------------------------------------------------------------------
// The native class
class Something
{
public:
    Something()
    {
        std::cout << "Something constructed" << std::endl;
		m_text = "(empty)";
    }

    ~Something()
    {
        std::cout << "Something destroyed" << std::endl;
    }

    void doStuff()
    {
        std::cout << "Hello I am something, and my text is " << m_text << std::endl;
    }

	const std::string & getText() { return m_text; }
	void setText(const std::string & str) { m_text = str; }

private:
	std::string m_text;
};

//------------------------------------------------------------------------------
// Binding
namespace
{
    int Something_doStuff(HSQUIRRELVM vm)
    {
        auto * self = getNativeInstance<Something>(vm, 1);
        self->doStuff();
        return 0;
    }

	int Something_getText(HSQUIRRELVM vm)
	{
		auto * self = getNativeInstance<Something>(vm, 1);
		sq_pushstring(vm, self->getText().c_str(), -1);
		return 1;
	}

	int Something_setText(HSQUIRRELVM vm)
	{
		auto * self = getNativeInstance<Something>(vm, 1);
        const char * str = getString(vm, 2);
		if (str)
			self->setText(str);
		return 0;
	}

    void registerSomething(HSQUIRRELVM vm)
    {
        const char * className = "Something";

        ScriptClass c(vm, className);
        c.setConstructor(createClassInstance<Something>);
        c.setMethod("doStuff", Something_doStuff);
		c.setMethod("setText", Something_setText);
		c.setMethod("getText", Something_getText);

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

