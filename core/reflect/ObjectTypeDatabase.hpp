/*
ObjectTypeDatabase.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_OBJECT_TYPE_DATABASE__
#define __HEADER_SN_OBJECT_TYPE_DATABASE__

#include <vector>
#include <unordered_map>
#include <core/util/assert.hpp>
#include <core/reflect/ObjectType.hpp>

namespace sn
{

typedef std::unordered_map<std::string, ObjectType*> ObjectTypeMap;

/// \brief Singleton class that allows the creation of objects from their name or ID.
/// It can be used for native serialization and basic reflection.
/// Type information is generally read-only.
/// \warning Not thread-safe yet.
class SN_API ObjectTypeDatabase
{
public:

    /// \brief Returns singleton
    static ObjectTypeDatabase & get();

    /// \brief Registers an object type that uses SN_OBJECT macro in its definition.
    /// \note it generates integer IDs from a counter, then as long as you call
    /// this function in the same order, these IDs will be the same even on different
    /// computers or platforms.
    /// However, IDs may differ if the version of the engine differ too, as there
    /// might be new objects.
    template <class Object_T>
    ObjectType & registerType()
    {
        std::string typeName = Object_T::__sGetClassName();
        std::string baseName = Object_T::__sGetBaseClassName();

        // Check if the object has already been registered
        SN_ASSERT(!isRegistered(typeName), "ObjectTypeDatabase::registerType: registered the same type twice! (" << typeName << ")");

        ObjectType * type = new ObjectType(typeName, baseName);

        //type->userData = userData;
        type->m_moduleName = m_currentModule;

        // Generate type ID
        type->m_ID = m_nextID++;

        // Register factory function
        type->m_factory = Object_T::instantiate;

		type->m_isAbstract = std::is_abstract<Object_T>();

        // Register type
        m_registeredTypes[type->getName()] = type;

#ifdef SN_BUILD_DEBUG
        SN_DLOG("Registered " << type->toString());
#endif
        return *type;
    }

    template <class Object_T>
    void unregisterType()
    {
        ObjectType & type = Object_T::sObjectType();
        unregisterType(type);
    }

    void unregisterType(ObjectType & t);
    bool isRegistered(const std::string & typeName);

    /// \brief Gets an object metaclass from its name
    /// \return the object type, or null if not found.
    ObjectType * getType(const std::string & typeName);

    /// \brief Gets an object metaclass from its name, and produces an error if it was not found.
    /// \return a constant reference to the object type.
    const ObjectType & getTypeConstRef(const std::string & typeName);

    inline const ObjectTypeMap & getTypes() const { return m_registeredTypes; }

    /// \brief Gets all object types defined into a module
    /// \param modName: name of the module
    /// \param out_types: list in which results will be added
    void getTypesByModuleName(const std::string & modName, std::vector<const ObjectType*> & out_types) const;

    void beginModule(const std::string & name);
    void endModule();
    void unregisterModule(const std::string & name);

    void clear();

private:

    // Private to prevent construction from the outside
    ObjectTypeDatabase() :
        m_nextID(1) // IDs start at 1, 0 means null
    {}

    ~ObjectTypeDatabase()
    {
        clear();
    }

    // Prevent copy-construction
    ObjectTypeDatabase(const ObjectTypeDatabase&);

    // Prevent assignment
    ObjectTypeDatabase & operator=(const ObjectTypeDatabase&);

private:

    // References to registered types
    ObjectTypeMap m_registeredTypes;

    std::string m_currentModule;

    u32 m_nextID;

};

} // namespace sn

#endif // __HEADER_SN_OBJECT_TYPE_DATABASE__

