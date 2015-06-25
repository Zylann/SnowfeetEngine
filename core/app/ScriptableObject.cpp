#include "ScriptableObject.hpp"

namespace sn
{

// Notes about the ownership system
//----------------------------------------

// Calling addRef() or creating an instance of a ScriptableObject means taking ownership of it.

// By default, ScriptableObjects that are created in C++ don't give ownership to Squirrel.
// If they are created from script, however, Squirrel has ownership, because it gets the first reference.
// Ownership can be switched by calling add/releaseScriptOwnership, however it shouldn't be
// required depending on the way you bind your classes (somehow making constructors private to the script writer).

// ScriptableObjects that are not owned by Squirrel will set the userpointer of their script object to null when they are destroyed.
// This means the "self" pointer must always be checked when a method call is performed.

// ScriptableObjects that are owned by Squirrel can still be present in the VM even after the scene is destroyed,
// if they are stored in globals for instance.

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

