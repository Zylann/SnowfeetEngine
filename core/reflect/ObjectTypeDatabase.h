/*
ObjectTypeDatabase.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_OBJECT_TYPE_DATABASE__
#define __HEADER_SN_OBJECT_TYPE_DATABASE__

#include <vector>
#include <unordered_map>
#include <core/util/assert.h>
#include <core/reflect/ObjectType.h>

namespace sn
{

typedef std::unordered_map<std::string,const ObjectType*> ObjectTypeMap;
typedef std::vector<const ObjectType*> ObjectTypeList;

/// \brief Singleton class that allows the creation of objects from their name or ID.
/// It can be used for native serialization and basic reflection.
/// Type information is generally read-only.
/// \warning Not thread-safe yet.
class SN_API ObjectTypeDatabase
{
public:

    /// \brief Returns singleton
    static ObjectTypeDatabase & get();

    /// \brief Registers a type without base class
    template <class Object_T>
    ObjectType & registerType(const char * fullName, const char * scriptName = nullptr)
    {
        // Check if the object has already been registered
        SN_ASSERT(!isRegistered(fullName), "ObjectTypeDatabase::registerType: registered the same type twice! (" << fullName << ")");

        ObjectType & type = Object_T::s_objectType;

        //type->userData = userData;
        type.m_moduleName = m_currentModule;
        type.setName(fullName);

        if (scriptName)
            type.setScriptName(scriptName);

        // Generate type ID
        type.m_ID = m_types.size();

        // Register factory function
        type.m_factory = sn::instantiateOrNull<Object_T>;

		type.m_isAbstract = std::is_abstract<Object_T>();

        // Register type
        m_typesByName[type.getName()] = &type;
        m_types.push_back(&type);

#ifdef SN_BUILD_DEBUG
        SN_DLOG("Registered " << type.toString());
#endif
        return type;
    }

    /// \brief Registers a type with one base class
    template <class Object_T, class Base_T>
    ObjectType & registerType(const char * fullName, const char * scriptName = nullptr)
    {
        // Check inheritance at compile-time
        static_assert(std::is_base_of<Base_T, Object_T>::value, "Object doesn't derives from base");

        ObjectType & type = registerType<Object_T>(fullName, scriptName);

        type.r_base = &sn::getObjectType<Base_T>();

        return type;
    }

    template <class Object_T>
    void unregisterType()
    {
        const ObjectType & type = sn::getObjectType<Object_T>();
        unregisterType(type);
    }

    void unregisterType(const ObjectType & t);
    bool isRegistered(const std::string & typeName);

    /// \brief Gets an object metaclass from its name
    /// \return the object type, or null if not found.
    const ObjectType * getType(const std::string & typeName);

    /// \brief Gets an object metaclass from its ID
    /// \return the object type, or null if not found.
    const ObjectType * getType(u32 id);

    inline const ObjectTypeList & getTypes() const { return m_types; }

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
    ObjectTypeDatabase();
    ~ObjectTypeDatabase();

    // Prevent copy-construction
    ObjectTypeDatabase(const ObjectTypeDatabase&);

    // Prevent assignment
    ObjectTypeDatabase & operator=(const ObjectTypeDatabase&);

private:

    // References to registered types
    ObjectTypeList m_types;
    ObjectTypeMap m_typesByName;

    std::string m_currentModule;

};

} // namespace sn

#endif // __HEADER_SN_OBJECT_TYPE_DATABASE__

