#include "ScriptableObject.hpp"

namespace sn
{

// Notes about the ownership system
//----------------------------------------

// If C++ releases all its references to the ScriptableObject, if refCount is 1, the ownership will go to Squirrel.
// If squirrel loses reference of the object, the release hook will get called and ownership will go to C++.
// When one of the two receives complete ownership and refcount reaches zero, the instance will be destroyed.

// Note: if an object (for example, Entity) gets released from C++ as a result of calling a destroy() function,
// the instance will still exist (in a somehow unusable state) as long as it is referenced somewhere in a script.
// FYI: this is similar to what happens in Unity3D when you Destroy() a game object.

// Note 2 for the future: why not use the HSQOBJECT as reference counter instead of RefCounted?
// It would make the engine strongly dependent on Squirrel, thought...

//------------------------------------------------------------------------------
SQInteger ScriptableObject::cb_releaseHook(SQUserPointer ptr, SQInteger size)
{
    auto * self = static_cast<ScriptableObject*>(ptr);
    SN_ASSERT(self != nullptr, "self is null in release hook");
    self->onReleaseFromScript();
    return 0;
}

//------------------------------------------------------------------------------
ScriptableObject::ScriptableObject() : RefCounted(), m_vm(0)
{
    sq_resetobject(&m_scriptSide);
}

//------------------------------------------------------------------------------
ScriptableObject::~ScriptableObject()
{
}

//------------------------------------------------------------------------------
void ScriptableObject::pushScriptObject(HSQUIRRELVM vm)
{
    if (sq_isnull(m_scriptSide))
    {
        SN_ASSERT(m_vm == nullptr || m_vm == vm, "VM Switch not implemented yet");

        // Get object type by reflection
        const ObjectType & ot = getObjectType();
        const std::string & bindClassName = ot.getScriptName();

        // Create instance without calling constructor (which would create a new C++ instance).
        // We'll store the object as a weak reference, so we shouldn't end up with a cycle.
        m_vm = vm;
        squirrel::Instance::createNoRef(m_vm, bindClassName, &m_scriptSide, false);
        // Push it on the stack (done by createNoRef)
        //sq_pushobject(vm, m_scriptSide.getObject());

        // Sets its pointer (because we didn't called the constructor)
        sq_setinstanceup(vm, -1, this);
        // Set the release hook to be notified when Squirrel looses track of the object
        sq_setreleasehook(vm, -1, ScriptableObject::cb_releaseHook);

        // Addref the C++ instance the first time to give Squirrel a shared ownership.
        addRef();
    }
    else
    {
        SN_ASSERT(m_vm == vm, "Invalid VM");

        // Squirrel already has shared ownership on our instance, so we just push it
        sq_pushobject(vm, m_scriptSide);
    }
}

//------------------------------------------------------------------------------
void ScriptableObject::onCreateFromScript(HSQUIRRELVM vm, HSQOBJECT obj)
{
    SN_ASSERT(sq_isnull(m_scriptSide), "Invalid state, m_scriptSide should be null");
    SN_ASSERT(m_vm == nullptr || m_vm == vm, "Multi-VM ownership is not supported");
    m_vm = vm;
    // Just keep a weak reference so we can push the same object from C++ when needed
    m_scriptSide = obj;
}

//------------------------------------------------------------------------------
void ScriptableObject::onReleaseFromScript()
{
    // Release the ownership Squirrel had on the C++ instance
    if (release()) // Test if returned _native_ refCount is > 0
    {
        // If there is still a reference in C++, just reset the dangling object handle.
        // (It will be re-created if we need to push the object in Squirrel again).
        sq_resetobject(&m_scriptSide);
    }
    //else
    //{
    //    // the ScriptableObject has been destroyed
    //}
}

} // namespace sn

