/*
types.h
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_TYPES__
#define __HEADER_SN_TYPES__

#include <core/config.h>

namespace sn
{

// 8 bits integer types
typedef signed char s8;
typedef unsigned char u8;

// 16 bits integer types
typedef signed short s16;
typedef unsigned short u16;

// 32 bits integer types
typedef signed int s32;
typedef unsigned int u32;

// 64 bits integer types
#if defined(_MSC_VER)
typedef signed __int64 s64;
typedef unsigned __int64 u64;
#else
typedef signed long long s64;
typedef unsigned long long u64;
#endif

// Float types
typedef float f32;
typedef double f64;

} // namespace sn

#endif // __HEADER_SN_TYPES__


