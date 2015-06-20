#include "Object.h"

namespace squirrel
{

//------------------------------------------------------------------------------
Object::Object(HSQUIRRELVM vm):
    m_vm(vm)
{
    sq_resetobject(&m_object);
}

//------------------------------------------------------------------------------
Object::Object(HSQUIRRELVM vm, HSQOBJECT obj):
    m_vm(vm), m_object(obj)
{
}

//------------------------------------------------------------------------------
Object::Object(const Object & other): 
    m_vm(other.m_vm), m_object(other.m_object)
{
    sq_addref(m_vm, &m_object);
}

//------------------------------------------------------------------------------
Object::~Object()
{
    releaseObject();
}

//------------------------------------------------------------------------------
bool Object::isNull() const
{
    return m_object._unVal.pUserPointer == nullptr && m_object._type == OT_NULL;
}

//------------------------------------------------------------------------------
Object& Object::operator=(const Object& other)
{
    releaseObject();
    m_vm = other.m_vm;
    m_object = other.m_object;
    if (!isNull())
        sq_addref(m_vm, &m_object);
    return *this;
}

//------------------------------------------------------------------------------
void Object::releaseObject()
{
    if (!isNull())
    {
        auto vm = m_vm;
        // Note: if the refcount is already zero, this has no effect
        //SQUnsignedInteger rc = sq_getrefcount(vm, &m_object);
        sq_release(vm, &m_object);
        sq_resetobject(&m_object);
    }
}

} // namespace squirrel

