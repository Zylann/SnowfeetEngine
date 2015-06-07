#include "Script.h"
#include <core/util/assert.hpp>

namespace sn
{

//------------------------------------------------------------------------------
Script::Script(HSQUIRRELVM vm) : ScriptObject(vm)
{
}

//------------------------------------------------------------------------------
bool Script::compileString(
    const std::string & sourceCode, 
    const std::string & scriptName)
{
    releaseObject();

    if(SQ_FAILED(sq_compilebuffer(m_vm, 
        sourceCode.c_str(), 
        static_cast<SQInteger>(sourceCode.size()),
        scriptName.c_str(), 
        true // raise error
        )))
    {
        return false;
    }

    sq_getstackobj(m_vm, -1, &m_object);
    sq_addref(m_vm, &m_object);
    sq_pop(m_vm, 1);

    return true;
}

//------------------------------------------------------------------------------
bool Script::execute()
{
    SN_ASSERT(!isNull(), "Script is null");

    SQInteger top = sq_gettop(m_vm);
    sq_pushobject(m_vm, m_object);
    sq_pushroottable(m_vm);

    SQRESULT result = sq_call(m_vm, 1, false, true);

    sq_settop(m_vm, top);
    return !SQ_FAILED(result);
}

} // namespace sn

