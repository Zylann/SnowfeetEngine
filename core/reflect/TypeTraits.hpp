#ifndef __HEADER_SN_REFLECT_TYPE_TRAITS__
#define __HEADER_SN_REFLECT_TYPE_TRAITS__

#include <core/types.hpp>
#include <core/math/Color.hpp>
#include <core/math/Vector.hpp>
#include <core/math/Area.hpp>
#include <string>

namespace sn
{

class Object;

// - The standard typeid can differ across assemblies and executions,
//   and hash codes can be the same for two types.
// - Automatic template hacks don't work too, 
//   because they would be implemented several times across assemblies,
//   and ID collisions could occur as well.
// So at the moment, I think it's better to hardcode IDs for most known types...

template <typename T>
struct TypeTraits { };

//------------------------------------------------------
// Primitives

template <> struct TypeTraits<bool> { static const u32 ID = 1; };

template <> struct TypeTraits<u8> { static const u32 ID = 2; };
template <> struct TypeTraits<u16> { static const u32 ID = 3; };
template <> struct TypeTraits<u32> { static const u32 ID = 4; };
template <> struct TypeTraits<u64> { static const u32 ID = 5; };

template <> struct TypeTraits<s8> { static const u32 ID = 6; };
template <> struct TypeTraits<s16> { static const u32 ID = 7; };
template <> struct TypeTraits<s32> { static const u32 ID = 8; };
template <> struct TypeTraits<s64> { static const u32 ID = 9; };

template <> struct TypeTraits<f32> { static const u32 ID = 10; };
template <> struct TypeTraits<f64> { static const u32 ID = 11; };

//------------------------------------------------------
// Standard

template <> struct TypeTraits<std::string> { static const u32 ID = 20; };

//------------------------------------------------------
// Core

template <> struct TypeTraits<Color> { static const u32 ID = 30; };

template <> struct TypeTraits<Vector<s32, 2>> { static const u32 ID = 31; };
template <> struct TypeTraits<Vector<f32, 2>> { static const u32 ID = 32; };

template <> struct TypeTraits<Vector<s32, 3>> { static const u32 ID = 33; };
template <> struct TypeTraits<Vector<f32, 3>> { static const u32 ID = 34; };

template <> struct TypeTraits<Vector<s32, 4>> { static const u32 ID = 35; };
template <> struct TypeTraits<Vector<f32, 4>> { static const u32 ID = 36; };

template <> struct TypeTraits<Area<s32, 2>> { static const u32 ID = 37; };
template <> struct TypeTraits<Area<f32, 2>> { static const u32 ID = 38; };

template <> struct TypeTraits<Area<s32, 3>> { static const u32 ID = 39; };
template <> struct TypeTraits<Area<f32, 3>> { static const u32 ID = 40; };

// More complex classes must derive from Object
template <> struct TypeTraits<Object> { static const u32 ID = 50; };

//------------------------------------------------------
// Types with modifiers

template <typename T> struct TypeTraits<T*> { static const u32 ID = TypeTraits<T>::ID | 0x0001ffff; };
template <typename T> struct TypeTraits<T&> { static const u32 ID = TypeTraits<T>::ID | 0x0002ffff; };

//------------------------------------------------------
template <typename T>
inline u32 getTypeID()
{
    return TypeTraits<T>::ID;
}

} // namespace sn

#endif // __HEADER_SN_REFLECT_TYPE_TRAITS__

