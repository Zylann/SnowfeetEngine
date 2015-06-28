/*
ScriptableObject.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SCRIPTABLEOBJECT__
#define __HEADER_SN_SCRIPTABLEOBJECT__

#include <core/reflect/Object.hpp>
#include <core/util/RefCounted.hpp>
#include <core/squirrel/Instance.h>
#include <core/squirrel/Class.h>
#include <core/squirrel/Table.h>
#include <type_traits>

#define SN_SCRIPT_OBJECT(_className, _baseName) SN_OBJECT(_className, _baseName)

namespace sn
{

/// \brief All classes accessible and creatable from scripts should inherit this class.
/// It is an Object because reflection is currently used to get the class name used in scripts.
/// It is RefCounted because of shared ownership between C++ and scripts.
class SN_API ScriptableObject : public RefCounted, public Object
{
public:
    SN_SCRIPT_OBJECT(sn::ScriptableObject, sn::Object)

    ScriptableObject();

    /// \brief Pushes the C++ instance on the Squirrel stack.
    /// Several calls should return the same Squirrel instance, as long as at least one stays referenced.
    /// If no class instance is referenced by Squirrel at the time, a new one will be created.
    bool pushScriptObject(HSQUIRRELVM vm, bool giveOwnership=false);

    /// \brief Releases ownership from Squirrel.
    /// release() will be called, so if you hold no other reference to the object, it will be destroyed.
    /// \return reference count
    u32 releaseScriptOwnership();

    void addScriptOwnership();

	// TODO Option to prevent constructing classes directly from script,
	// Because it would first give ownership to Squirrel, and sometimes we don't want that

    /// \brief Auto-binds a ScriptableObject class to Squirrel with just a constructor and destructor,
    /// and returns it to let binding the other specific members.
    template <typename T>
    static squirrel::Class bindBase(HSQUIRRELVM vm)
    {
        // Get the class name
        const ObjectType & ot = sn::getObjectType<T>();
        const std::string & sqName = ot.getScriptName();

        // Get the base class name
        std::string sqBaseName;
        const ObjectType * base = ot.getBase();
        const ObjectType & rootType = sn::getObjectType<ScriptableObject>();
        if (base && !base->isAbstract() && *base != rootType)
        {
            sqBaseName = base->getScriptName();
        }

        // Create the class
        squirrel::Class c(vm, sqName, sqBaseName);

        // Bind constructor
        ScriptableObject::bindBaseConstructor<T>(vm, c, std::is_abstract<T>());

        // Make it accessible from the root table
        squirrel::RootTable(vm).setObject(sqName.c_str(), c);

        return c;
    }

protected:
    virtual ~ScriptableObject();

private:
    /// \brief This should be called when the ScriptableObject is created from a script.
    void onCreateFromScript(HSQUIRRELVM vm, HSQOBJECT obj);

    /// \brief This should be called when the ScriptableObject is released from a script.
    void onReleaseFromScript();

    template <typename T>
    static void bindBaseConstructor(HSQUIRRELVM vm, squirrel::Class & c, std::false_type isAbstract)
    {
        c.setConstructor(cb_scriptConstructor<T>);
    }

    template <typename T>
    static void bindBaseConstructor(HSQUIRRELVM vm, squirrel::Class & c, std::true_type isAbstract)
    {
        c.setPrivateConstructor();
    }

    /// \brief Must be bound as native constructor of the class in Squirrel
    template <typename T>
    static SQInteger cb_scriptConstructor(HSQUIRRELVM vm)
    {
		// Check class type
        typedef std::is_base_of<ScriptableObject, T> TestBaseOfT;
        SN_STATIC_ASSERT_MSG(TestBaseOfT::value, "Class does not derives from ScriptableObject");

		// Create C++ instance and put a reference inside the Squirrel object
        T * self = new T();
        sq_setinstanceup(vm, -1, self);
        sq_setreleasehook(vm, -1, cb_releaseHook);

		// Notify
        HSQOBJECT obj;
        sq_getstackobj(vm, -1, &obj);
        self->onCreateFromScript(vm, obj);

        return 0;
    }

    /// \brief Must be bound as release hook of instances in Squirrel
    static SQInteger cb_releaseHook(SQUserPointer ptr, SQInteger size);

private:
    /// \brief VM owning the object. Can be null if the object is not referenced in a script.
    HSQUIRRELVM m_vm;

    /// \brief This is a _weak_ reference to the Squirrel side of the object.
    /// It becomes non-null when the C++ class is instantiated from a script,
    /// or when it needs to be pushed on the Squirrel stack for the first time.
    /// It allows the engine to ensure the Squirrel world will get the same
    /// instance when it needs to be pushed by C++.
    /// \warning Althought this could allow inheriting native classes, this is not a supported feature.
    HSQOBJECT m_scriptSide;

	/// \brief If true, Squirrel holds one of the RefCounted references.
    bool m_scriptOwnership;

};

} // namespace sn

#endif // __HEADER_SN_SCRIPTABLEOBJECT__

