#include "ScriptInstance.hpp"
#include "../app/Application.hpp"
#include "../squirrel/bind_tools.hpp"

namespace sn
{

ScriptInstance::ScriptInstance()
{
    // Initialize object to none
    sq_resetobject(&m_sqObject);
}

ScriptInstance::~ScriptInstance()
{
    destroy();
}

HSQUIRRELVM ScriptInstance::getVM() const
{
    return Application::get().getScriptManager().getVM();
}

bool ScriptInstance::create(const std::string & name)
{
    // Destroy previous instance if any
    destroy();

    // Parse the full name to extract namespaces
    std::vector<std::string> parts = split(name, '.');

    auto vm = getVM();
    sq_pushroottable(vm);

    // Push tables the class might be into
    for (u32 i = 0; i < parts.size() - 1; ++i)
    {
        // Push table name
        const std::string &tableName = parts[i];
        sq_pushstring(vm, _SC(tableName.c_str()), tableName.size());

        // Get table
        if (SQ_FAILED(sq_get(vm, -1)))
        {
            sq_pop(vm, 1); // Pop root table
            SN_ERROR("Squirrel class not found: '" << name << "'");
            return false;
        }
    }

    // Push class name
    const std::string & className = parts.back();
    sq_pushstring(vm, _SC(className.c_str()), className.size());

    // Get the class
    if (SQ_FAILED(sq_get(vm, -2))) // -2 because it's like we do roottable."className"
    {
        sq_pop(vm, 1);
        SN_ERROR("Squirrel class not found: '" << name << "'");
        return false;
    }

    // Create an instance of it (It's like calling a function)
    sq_pushroottable(vm); // this
    if (SQ_FAILED(sq_call(vm, 1, SQTrue, SQTrue)))
    {
        sq_pop(vm, 1);
        SN_ERROR("Squirrel class instantiation raised an error");
        return false;
    }

    // Reference the object
    sq_getstackobj(vm, -1, &m_sqObject);
    sq_addref(vm, &m_sqObject);

    // Pops instance, class and roottable
    sq_pop(vm, 3);

    return true;
}

bool ScriptInstance::isNull() const
{
    return m_sqObject._unVal.pUserPointer == NULL && m_sqObject._type == OT_NULL;
}

bool ScriptInstance::destroy()
{
    if (!isNull())
    {
        auto vm = getVM();
        sq_release(vm, &m_sqObject);
        sq_resetobject(&m_sqObject);
    }
    return true;
}

Variant ScriptInstance::getProperty(const std::string & name)
{
    // TODO
    return Variant();
}

bool ScriptInstance::hasMethod(const std::string & name)
{
    // TODO
    return false;
}

bool ScriptInstance::callMethod(const std::string & methodName)
{
    if (isNull())
        return false;

    auto vm = getVM();

    sq_pushobject(vm, m_sqObject);
    sq_pushstring(vm, methodName.c_str(), methodName.size());

    // Get the method
    if (SQ_FAILED(sq_get(vm, -2)))
    {
        sq_pop(vm, -1);
        return false;
    }

    // Push this
    sq_pushobject(vm, m_sqObject);

    // Call the method
    if (SQ_FAILED(sq_call(vm, 1, SQFalse, SQTrue)))
    {
        sq_pop(vm, -1);
        SN_ERROR("Squirrel error on method call");
        return false;
    }

    // Pop the function and the roottable
    sq_pop(vm, 2);

    return true;
}

} // namespace sn

