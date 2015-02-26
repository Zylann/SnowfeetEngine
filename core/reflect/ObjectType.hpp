/*
ObjectType.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_OBJECTTYPE__
#define __HEADER_SN_OBJECTTYPE__

#include <string>
#include <sstream>
#include <functional>
#include <core/types.hpp>

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
    ) :
        m_ID(0), // null ID, until the type gets registered
        m_name(p_name),
        m_baseName(p_baseName)
    {}

    //~ObjectType()
    //{
    //}

    bool is(const std::string & typeName, bool includeInheritance=true) const;
    bool is(const ObjectType & other, bool includeInheritance=true) const;

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

	void print(std::ostream & os) const
    {
        os << "{[" << m_ID << "]" << m_name << " : " << m_baseName << "}";
    }

    std::string toString() const
    {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

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

    std::function<Object*()> m_factory;

};

bool SN_API operator==(const ObjectType & a, const ObjectType & b);
bool SN_API operator!=(const ObjectType & a, const ObjectType & b);


} // namespace sn

#endif // __HEADER_SN_OBJECTTYPE__

