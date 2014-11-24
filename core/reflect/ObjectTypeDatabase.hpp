#ifndef __HEADER_SN_OBJECT_TYPE_DATABASE__
#define __HEADER_SN_OBJECT_TYPE_DATABASE__

#include <vector>
#include <unordered_map>
#include <functional>
#include <core/util/assert.hpp>
#include <core/reflect/Object.hpp>

namespace sn
{

/// \brief Singleton class that allows the creation of objects from their name or ID.
/// It can be used for native serialization and basic reflection.
class SN_API ObjectTypeDatabase
{
private:

    // Private to prevent construction from the outside
    ObjectTypeDatabase() :
        m_nextID(1) // IDs start at 1
    {}

    // Prevent copy-construction
    ObjectTypeDatabase(const ObjectTypeDatabase&);

    // Prevent assignment
    ObjectTypeDatabase & operator=(const ObjectTypeDatabase&);

public:

    ///// \brief Returns singleton
    static ObjectTypeDatabase & get();

    /// \brief Registers an object type that uses SN_OBJECT macro in its definition.
    /// \param userData: optional type information (ObjectType will take the ownership on it!)
    /// \note it generates integer IDs from a counter, then as long as you call
    /// this function in the same order, these IDs will be the same even on different
    /// computers or platforms.
    /// However, IDs may differ if the version of the engine differ too, as there
    /// might be new objects.
    template <class Object_T>
    void registerType(IObjectTypeUserData * userData=nullptr)
    {
        // Get type structure from a static function defined in the object class
        // (that's the case if it uses the SN_OBJECT macro)
        ObjectType & type = Object_T::sObjectType();

        // Check if the object has already been registered
        SN_ASSERT(!isRegistered(type), "ObjectTypeDatabase::registerType: registered the same type twice ! (" << type.toString() << ")");

        type.userData = userData;
        type.moduleName = m_currentModule;

        // Generate type ID
        type.ID = m_nextID++;
        m_nameToID[type.name] = type.ID;

        // Register factory function
        m_factories[type.name] = Object_T::instantiate;

        // Register type
        if(type.ID >= m_registeredTypes.size())
        {
            m_registeredTypes.resize(type.ID+1, nullptr);
        }
        m_registeredTypes[type.ID] = &type;

#ifdef SN_BUILD_DEBUG
        SN_DLOG("Registered " << type.toString());
#endif
    }

    template <class Object_T>
    void unregisterType()
    {
        ObjectType & type = Object_T::sObjectType();
        unregisterType(type);
    }

    void unregisterType(ObjectType & t);
    bool isRegistered(ObjectType & t);

    /// \brief Creates a new instance of an object from its name.
    /// It does the same thing as "new MyObject()", where className = "MyObject".
    /// \return pointer to dynamically allocated object instance, or null if the object
    /// couldn't be allocated (as for abstract types).
    Object * instantiate(const std::string className);

    /// \brief Gets an object metaclass from its name
    /// \return the object type, or null if not found.
    ObjectType * getType(const std::string & typeName);

    void beginModule(const std::string & name);
    void endModule();
    void unregisterModule(const std::string & name);

private:

    // References to registered types
    std::vector<ObjectType*> m_registeredTypes;

    std::string m_currentModule;

    std::unordered_map<std::string, ObjectTypeID> m_nameToID;
    std::unordered_map<std::string, std::function<Object*()>> m_factories;
    u32 m_nextID;

};

} // namespace sn

#endif // __HEADER_SN_OBJECT_TYPE_DATABASE__

