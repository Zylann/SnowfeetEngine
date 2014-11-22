/*
typecheck.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_TYPECHECK__
#define __HEADER_SN_TYPECHECK__

#include <typeinfo>
#include <core/config.hpp>
#include <core/util/Log.hpp>

namespace sn
{

/// \brief Performs a dynamic_cast with a meaningful error in debug mode,
/// and a static_cast in release.
/// Note: B may be a pointer type.
template <typename B, typename A>
inline B checked_cast(A * a)
{
#ifdef SN_BUILD_DEBUG
    // Slow and explicit check
    B castedPointer = dynamic_cast<B>(a);
    if(castedPointer == nullptr)
    {
        // Print an error, and attempt to write runtime types information.
        // Note that typeid(B).name() might not always return meaningful information,
        // as typeid() only has to return different values (whatever they are)
        // for different types, as defined in C++ specs.
        SN_ERROR("Failed to cast pointer ("
            "from {" << (a==nullptr ? "nullptr" : typeid(a).name()) << "} "
            "to {" << typeid(B).name() << "}");
    }
    // Note: no crash here, because debugging would always lead to this file
    // and wouldn't be helpful.
    //assert(castedPointer);
    return castedPointer;
#else
    // Fast
    return static_cast<B>(a);
#endif
}

} // namespace sn

#endif // __HEADER_SN_TYPECHECK__


