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
ScriptableObject::ScriptableObject() : RefCounted(), m_vm(0), m_scriptOwnership(false)
{
    sq_resetobject(&m_scriptSide);
}

//------------------------------------------------------------------------------
ScriptableObject::~ScriptableObject()
{
    // Note: the following only works if m_scriptSide is updated accordingly.
    // (Horrible crashes occur if it's not done properly)

    // If m_scriptSide still exists
    if (!sq_isnull(m_scriptSide))
    {
        // Reset its userpointer
        sq_pushobject(m_vm, m_scriptSide);
        sq_setinstanceup(m_vm, -1, nullptr);
        sq_pop(m_vm, 1);
    }
}

//------------------------------------------------------------------------------
bool ScriptableObject::pushScriptObject(HSQUIRRELVM vm, bool giveOwnership)
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
        if (!squirrel::Instance::createNoRef(m_vm, bindClassName, &m_scriptSide, false))
        {
            SN_ERROR("The class " << ot.getName() << " seems not bound to Squirrel");
            sq_pushnull(vm);
            return false;
        }

        // Push it on the stack (done by createNoRef)
        //sq_pushobject(vm, m_scriptSide.getObject());

        // Sets its pointer (because we didn't called the constructor)
        sq_setinstanceup(vm, -1, this);

        // Set the release hook to be notified when Squirrel looses track of the object.
        // We need this to destroy the C++ instance OR reset m_scriptSide when needed.
        sq_setreleasehook(vm, -1, ScriptableObject::cb_releaseHook);

        if (giveOwnership && !m_scriptOwnership)
        {
            // Addref the C++ instance the first time to give Squirrel a shared ownership.
            addRef();
            m_scriptOwnership = true;
        }
    }
    else
    {
        SN_ASSERT(m_vm == vm, "Invalid VM");

        // Squirrel already has shared ownership on our instance, so we just push it
        sq_pushobject(vm, m_scriptSide);
    }

    return true;
}

//------------------------------------------------------------------------------
u32 ScriptableObject::releaseScriptOwnership()
{
    if (m_scriptOwnership && !sq_isnull(m_scriptSide))
    {
        m_scriptOwnership = false;
        return release();
    }
    return 0;
}

//------------------------------------------------------------------------------
void ScriptableObject::addScriptOwnership()
{
    if (!m_scriptOwnership && !sq_isnull(m_scriptSide))
    {
        m_scriptOwnership = true;
        addRef();
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
    // Now Squirrel has ownership on the object
    m_scriptOwnership = true;
}

//------------------------------------------------------------------------------
void ScriptableObject::onReleaseFromScript()
{
    if (m_scriptOwnership) 
    {
        // Release the ownership Squirrel had on the C++ instance
        if (release()) // Test if returned _native_ refCount is > 0
        {
            // If there is still a reference in C++, just reset the dangling object handle.
            // (It will be re-created if we need to push the object in Squirrel again).
            sq_resetobject(&m_scriptSide);
            m_scriptOwnership = false;
        }
        //else
        //{
        //    // the ScriptableObject has been destroyed
        //}
    }
    else
    {
        sq_resetobject(&m_scriptSide);
    }
}

} // namespace sn

