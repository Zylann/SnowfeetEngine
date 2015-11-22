#include "Object.h"
#include <core/util/assert.h>

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

//------------------------------------------------------------------------------
Object & Object::setMethod(const char * methodName, SQFUNCTION cb_method, SQInteger nparams, const std::string & a_paramsMask, SQBool isStatic)
{
    SN_ASSERT(!isNull(), "Object is null");
    SN_ASSERT(cb_method != nullptr, "Function pointer argument is null");

    sq_pushobject(m_vm, m_object);
    sq_pushstring(m_vm, methodName, -1);
    sq_newclosure(m_vm, cb_method, 0);

    if (nparams != NO_PARAMCHECK)
    {
        sq_setparamscheck(m_vm, nparams, a_paramsMask.c_str());
    }

    // Store the method
    sq_newslot(m_vm, -3, isStatic);

    sq_pop(m_vm, 1);

    return *this;
}
} // namespace squirrel

