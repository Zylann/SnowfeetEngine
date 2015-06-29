/*
ObjectType.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_OBJECTTYPE__
#define __HEADER_SN_OBJECTTYPE__

#include <string>
#include <functional>
#include <unordered_map>

#include <core/types.h>
#include <core/reflect/ObjectProperty.h>

namespace sn
{

typedef u16 ObjectTypeID;

class Object;

/// \brief Meta-class of an object.
class SN_API ObjectType
{
public:
    ObjectType(
        const std::string & p_name,
        const std::string & p_baseName
    );

    ~ObjectType()
    {
        for (auto it = m_properties.begin(); it != m_properties.end(); ++it)
            delete it->second;
    }

    // TODO Handle setters returning *this for chaining
    template <class C, typename T>
    ObjectType & addProperty(const std::string & name, T(C::*getterMethod)(), void(C::*setterMethod)(T))
    {
        SN_ASSERT(!hasProperty(name), "Property " << name << " registered twice on ObjectType " << toString());
        ObjectProperty * property = new ObjectProperty(name);
        if (getterMethod)
            property->setGetterMethod(getterMethod);
        if (setterMethod)
            property->setSetterMethod(setterMethod);
        m_properties[name] = property;
        return *this;
    }

    template <class C, typename T>
    ObjectType & addProperty(const std::string & name, T(C::*getterMethod)())
    {
        SN_ASSERT(!hasProperty(name), "Property " << name << " registered twice on ObjectType " << toString());
        ObjectProperty * property = new ObjectProperty(name);
        if (getterMethod)
            property->setGetterMethod(getterMethod);
        m_properties[name] = property;
        return *this;
    }

    bool hasProperty(const std::string & name) const;
    const ObjectProperty * getProperty(const std::string & name) const;

    bool is(const std::string & typeName, bool includeInheritance=true) const;
    bool is(const ObjectType & other, bool includeInheritance=true) const;

	bool derivesFrom(const ObjectType & other) const;

    /// \brief Creates a new instance of an object from its name.
    /// It does the same thing as "new MyObject()", where className = "MyObject".
    /// \return pointer to dynamically allocated object instance, or null if the object
    /// couldn't be allocated (as for abstract types).
    Object * instantiate() const;

	inline bool isAbstract() const { return m_isAbstract; }
	inline ObjectTypeID getID() const { return m_ID; }
	inline const std::string & getName() const { return m_name; }
	inline const std::string & getBaseName() const { return m_baseName; }
	inline const std::string & getModuleName() const { return m_moduleName; }

    /// \brief Gets the base type of this one.
    /// \return Base type, or nullptr if there is none.
    const ObjectType * getBase() const;

	void getChildrenTypes(std::vector<const ObjectType*> & out_children) const;

    std::string toString() const;

    void setScriptName(const std::string & fullClassName) { m_scriptName = fullClassName; }
    const std::string & getScriptName() const { return m_scriptName; }

private:
	friend class ObjectTypeDatabase;

	bool m_isAbstract;

	/// \brief Unique numeric ID of the component.
    /// DO NOT modify (only done on type registration).
    /// The value of IDs may be generated sequentially, starting from 1.
    /// 0 means null type.
    /// \warning IDs can change between two builds of the engine.
    /// for an absolute version-safe comparison, use type names instead.
    ObjectTypeID m_ID;

    /// \brief Full unique name of the type.
    std::string m_name;

    /// \brief Name of the base class.
    std::string m_baseName;

    /// \brief Optional name of the module this object type is registered in.
    std::string m_moduleName;

    /// \brief Optional name given in a scripting environment.
    std::string m_scriptName;

    /// \brief Closure responsible for object instances creation
    std::function<Object*()> m_factory;

    std::unordered_map<std::string, ObjectProperty*> m_properties;

};

bool SN_API operator==(const ObjectType & a, const ObjectType & b);
bool SN_API operator!=(const ObjectType & a, const ObjectType & b);


} // namespace sn

#endif // __HEADER_SN_OBJECTTYPE__

