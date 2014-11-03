#ifndef __HEADER_SN_OBJECT__
#define __HEADER_SN_OBJECT__

#include <core/reflect/ObjectType.hpp>

// Constraints of the reflection system:
// Classes must define a parameter-less constructor
// Single inheritance only
// Type IDs must be the same on whatever platform or compiler
// Must be usable from application scripts (not only native code)

#define _SN_STATIC_STRING(_name, _value)                                       \
    static const char * _name() {                                              \
        static const char * str = _value;                                      \
        return str;                                                            \
    }

#define _SN_OBJECT(_name, _baseName)                                           \
    _SN_STATIC_STRING(__sClassName, #_name)                                    \
    _SN_STATIC_STRING(__sBaseClassName, #_baseName)                            \
    static sn::ObjectType & sObjectType();                                     \
    const sn::ObjectType & objectType() const override {                       \
        return sObjectType();                                                  \
    }

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

// Put this in your .cpp in order to implement reflection data
#define SN_OBJECT_IMPL(_className)                                             \
    sn::ObjectType & _className::sObjectType() {                               \
        static sn::ObjectType t(                                               \
            _className::__sClassName(),                                        \
            _className::__sBaseClassName());                                   \
        return t;                                                              \
    }

namespace sn
{

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

/// \brief Base class for all objects that want to have the reflection layer.
class SN_API Object
{
public:

    //SN_ABSTRACT_OBJECT(sn::Object,)

    /// \brief base metaclass.
    static sn::ObjectType & sObjectType();

    virtual ~Object() {}

    /// \brief get the metaclass of the object.
    virtual const sn::ObjectType & objectType() const
    {
        return sObjectType();
    }

    /// \brief Tests if the object is an instance of the given type.
    /// It also works for inherited types, if they share the same reflection system.
    template <class Object_T>
    bool isInstanceOf()
    {
        ObjectType & expectedType = Object_T::sObjectType();
        return objectType().is(expectedType);
    }

};

} // namespace sn

#endif // __HEADER_SN_OBJECT__




