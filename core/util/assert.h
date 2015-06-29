/*
assert.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSERT__
#define __HEADER_SN_ASSERT__

#include <core/util/Log.h>
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
//namespace sn
//{
//    template<bool> struct static_assert_util;
//    template<> struct static_assert_util<true> {};
//}
// sn::static_assert_util<(predicate)>()

// Generates a compile error if predicate is not true
#define SN_STATIC_ASSERT_MSG(predicate, msg) static_assert(predicate, msg)
#define SN_STATIC_ASSERT(predicate) static_assert(predicate, "Static assert failed: " #predicate)

#endif // __HEADER_SN_ASSERT__


