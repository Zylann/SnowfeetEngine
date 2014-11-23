#ifndef __HEADER_SN_ASSERT__
#define __HEADER_SN_ASSERT__

#include <core/util/Log.hpp>
#include <cassert>

//------------------------------------------------------------------------------
#define SN_ASSERT(predicate, expr) \
do{\
    if (!(predicate)){\
        SN_ERROR(expr); \
        assert(predicate); \
    }\
} while (false)

//------------------------------------------------------------------------------
template<bool> struct static_assert_util;
template<> struct static_assert_util<true> {};
// Generates a compile error if predicate is not true
#define SN_STATIC_ASSERT(predicate) static_assert_util<(predicate)>()

#endif // __HEADER_SN_ASSERT__


