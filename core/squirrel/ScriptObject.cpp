#include "ScriptObject.h"

namespace sn
{

//------------------------------------------------------------------------------
ScriptObject::ScriptObject():
    m_vm(nullptr)
{
    // Initialize object to none
    sq_resetobject(&m_object);
}

//------------------------------------------------------------------------------
ScriptObject::ScriptObject(HSQUIRRELVM vm):
    m_vm(vm)
{
    sq_resetobject(&m_object);
}

//------------------------------------------------------------------------------
ScriptObject::ScriptObject(const ScriptObject & other): m_vm(other.m_vm), m_object(other.m_object)
{
    sq_addref(m_vm, &m_object);
}

//------------------------------------------------------------------------------
ScriptObject::~ScriptObject()
{
    releaseObject();
}

//------------------------------------------------------------------------------
bool ScriptObject::isNull() const
{
    return m_object._unVal.pUserPointer == nullptr && m_object._type == OT_NULL;
}

//------------------------------------------------------------------------------
ScriptObject& ScriptObject::operator=(const ScriptObject& other)
{
    releaseObject();
    m_vm = other.m_vm;
    m_object = other.m_object;
    if (!isNull())
        sq_addref(m_vm, &m_object);
    return *this;
}

//------------------------------------------------------------------------------
void ScriptObject::releaseObject()
{
    if (!isNull())
    {
        auto vm = m_vm;
        // Note: if the refcount is already zero, this has no effect
        u32 rc = sq_getrefcount(vm, &m_object);
        sq_release(vm, &m_object);
        sq_resetobject(&m_object);
    }
}

} // namespace sn

