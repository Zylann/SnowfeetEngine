/*
Object.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_OBJECT__
#define __HEADER_SN_OBJECT__

#include <core/reflect/ObjectTypeDatabase.h>

// Constraints of the reflection system:
// - Classes must define a parameter-less constructor
// - Be able to map a class to its metaclass without hashing
// - Future support for multiple inheritance (mostly for interfaces)
// - Works with template classes (with different code)
// - Write as few code as possible
// - Run-time registration (dynamic modules)

/// \brief Write this in a public block of your class declaration.
/// \param _class: full-name of the class including the namespace (example: my::foo::Bar)
#define SN_OBJECT \
    static sn::ObjectType s_objectType;\
    virtual const sn::ObjectType & getObjectType() const {\
        return s_objectType;\
    }

/// \brief Write this in the implementation of your class.
/// It's important because it instantiates the type's singleton object.
/// \param _class: name of the class, the same you use to prefix method implementations
#define SN_OBJECT_IMPL(_class) \
    sn::ObjectType _class::s_objectType;

namespace sn
{

//------------------------------------------------------------------------------
template<class T>
T * instantiateOrNull_impl(std::true_type)
{
    return nullptr;
}
template<class T>
T * instantiateOrNull_impl(std::false_type)
{
    return new T();
}

/// \brief Tries to instantiate the given class dynamically.
/// \return pointer to the new instance, or null if the class was abstract.
template<class T>
T * instantiateOrNull()
{
    return instantiateOrNull_impl<T>(std::is_abstract<T>());
}

//------------------------------------------------------------------------------
/// \brief Base class for all objects that want to have the reflection layer.
class SN_API Object
{
public:
    SN_OBJECT

#ifdef SN_BUILD_DEBUG
    Object();
    virtual ~Object();
    static u32 getInstanceCount();
    static void printInstances(); // Warning: will not work if C++ modules are not loaded
#else
    virtual ~Object() {}
#endif

    template <class Object_T>
    bool isInstanceOf() const
    {
        const sn::ObjectType & expectedType = sn::getObjectType<Object_T>();
        return getObjectType().is(expectedType);
    }

    static sn::Object * instantiateObject()
    {
        // Object is abstract from reflection point of view
        return nullptr;
    }

	// TODO T should not be turned to a pointer type internally, for clarity
    /// \brief Casts the given object pointer to another Object-derived type known at compile-time.
    /// \param obj: pointer to the object instance to cast. If nullptr, the cast will fail.
    /// \return casted object on success, nullptr on fail
    template <class T>
    static T * cast(Object * obj)
    {
        if (obj == nullptr)
            return nullptr;
        const ObjectType & ot = obj->getObjectType();
        const ObjectType & otherType = sn::getObjectType<T>();
        if (ot.is(otherType, true))
            return static_cast<T*>(obj);
        else
            return nullptr;
    }

};

//------------------------------------------------------------------------------
// Helpers

/// \brief Gets the metaclass of an object class.
/// \note The goal of this function is to encapsulate the way the metaclass is stored.
template <class Object_T>
inline const ObjectType & getObjectType()
{
    return Object_T::s_objectType;
}

SN_API Object * instantiateDerivedObject(const std::string & typeName, const std::string & baseTypeName);
SN_API Object * instantiateDerivedObject(const std::string & typeName, const ObjectType & baseType);
SN_API Object * instantiateDerivedObject(const ObjectType & type, const ObjectType & baseType);

} // namespace sn

#endif // __HEADER_SN_OBJECT__




