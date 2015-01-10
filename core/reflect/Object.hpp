#ifndef __HEADER_SN_OBJECT__
#define __HEADER_SN_OBJECT__

#include <core/reflect/ObjectTypeDatabase.hpp>

// Constraints of the reflection system:
// - Classes must define a parameter-less constructor
// - Single inheritance only
// - Type IDs must be the same on whatever platform or compiler
// - You have to manually register types somewhere for C++ technical reasons

#define _SN_STATIC_STRING(_name, _value)                                       \
    static const char * _name() {                                              \
        static const char * str = _value;                                      \
        return str;                                                            \
    }

#define _SN_DECLARE_STATIC_GETOBJECTTYPE(_name)                                \
    static inline const sn::ObjectType & __sGetObjectType() {                  \
        return *sn::ObjectTypeDatabase::get().getType(__sGetClassName());      \
    }

#define _SN_DECLARE_GETOBJECTTYPE(_name)                                       \
    virtual const sn::ObjectType & getObjectType() const {                     \
        return __sGetObjectType();                                             \
    }

#define _SN_DECLARE_ISINSTANCEOF()                                             \
    template <class Object_T>                                                  \
    bool isInstanceOf() {                                                      \
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
    static sn::Object * instantiate() {                                        \
        return sn::instantiateOrNull<_name>();                                 \
    }

// Same as SN_OBJECT, but forces to abstract class. (The factory will return null).
// You may use it when your class is not abstract,
// but musn't be instantiated using reflection (or has a private constructor).
#define SN_ABSTRACT_OBJECT(_name, _baseName)                                   \
    _SN_OBJECT(_name, _baseName)                                               \
    static sn::Object * instantiate() {                                        \
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

    virtual ~Object() {}

    static sn::Object * instantiate()
    {
        // Object is abstract from reflection point of view
        return nullptr;
    }

};

//------------------------------------------------------------------------------
// Helpers

Object * instantiateDerivedObject(const std::string & typeName, const std::string & derivedTypeName);

} // namespace sn

#endif // __HEADER_SN_OBJECT__




