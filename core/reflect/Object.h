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
// - Single inheritance only
// - Type IDs must be the same on whatever platform or compiler
// - You have to manually register types somewhere for C++ technical reasons

// TODO use C++11 constexpr?
#define _SN_STATIC_STRING(_name, _value)                                       \
    static const char * _name() {                                              \
        static const char * str = _value;                                      \
        return str;                                                            \
    }

#define _SN_DECLARE_STATIC_GETOBJECTTYPE(_name)                                \
    static inline const sn::ObjectType & __sGetObjectType() {                  \
        return sn::ObjectTypeDatabase::get()                                   \
                    .getTypeConstRef(__sGetClassName());                       \
    }

#define _SN_DECLARE_GETOBJECTTYPE(_name)                                       \
    virtual const sn::ObjectType & getObjectType() const {                     \
        return __sGetObjectType();                                             \
    }

#define _SN_DECLARE_ISINSTANCEOF()                                             \
    template <class Object_T>                                                  \
    bool isInstanceOf() const {                                                \
        const sn::ObjectType & expectedType = Object_T::__sGetObjectType();    \
        return getObjectType().is(expectedType);                               \
    }

#define _SN_OBJECT(_name, _baseName)                                           \
    _SN_STATIC_STRING(__sGetClassName, #_name)                                 \
    _SN_STATIC_STRING(__sGetBaseClassName, #_baseName)                         \
    _SN_DECLARE_STATIC_GETOBJECTTYPE(_name)                                    \
    _SN_DECLARE_GETOBJECTTYPE(_name)                                           \
    _SN_DECLARE_ISINSTANCEOF()

// Put this declaration in public in a class inheriting from Object.
// Note: only single inheritance is supported.
// _name: class name with full namespace (must be unique !)
// _baseName: inherited class name with full namespace
#define SN_OBJECT(_name, _baseName)                                            \
    _SN_OBJECT(_name, _baseName)                                               \
    static sn::Object * instantiateObject() {                                  \
        return sn::instantiateOrNull<_name>();                                 \
    }

// Same as SN_OBJECT, but forces to abstract class. (The factory will return null).
// You may use it when your class is not abstract,
// but musn't be instantiated using reflection (or has a private constructor).
#define SN_ABSTRACT_OBJECT(_name, _baseName)                                   \
    _SN_OBJECT(_name, _baseName)                                               \
    static sn::Object * instantiateObject() {                                  \
        return nullptr;                                                        \
    }

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

    _SN_STATIC_STRING(__sGetClassName, "sn::Object")
    _SN_STATIC_STRING(__sGetBaseClassName, "")
    _SN_DECLARE_STATIC_GETOBJECTTYPE(sn::Object)
    _SN_DECLARE_GETOBJECTTYPE(sn::Object)
    _SN_DECLARE_ISINSTANCEOF()

#ifdef SN_BUILD_DEBUG
    Object();
    virtual ~Object();
    static u32 getInstanceCount();
    static void printInstances(); // Warning: will not work if C++ modules are not loaded
#else
    virtual ~Object() {}
#endif

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

    //static void registerReflectedMembers(ObjectType & ot);

};

//------------------------------------------------------------------------------
// Helpers

/// \brief Gets the metaclass of an object class.
/// \note The goal of this function is to encapsulate the way the metaclass is stored.
template <class Object_T>
inline const ObjectType & getObjectType()
{
    return Object_T::__sGetObjectType();
}

/// \brief Gets the name of an object class.
/// \note The goal of this function is to encapsulate the way the metaclass is stored.
template <class Object_T>
inline const char * getClassName()
{
    return Object_T::__sGetClassName();
}

Object * instantiateDerivedObject(const std::string & typeName, const std::string & derivedTypeName);
Object * instantiateDerivedObject(const std::string & typeName, const ObjectType & derivedType);
Object * instantiateDerivedObject(const ObjectType & type, const ObjectType & derivedType);

} // namespace sn

#endif // __HEADER_SN_OBJECT__




