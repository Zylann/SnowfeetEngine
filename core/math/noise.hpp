/*
noise.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_NOISE__
#define __HEADER_SN_NOISE__

#include <core/types.hpp>

namespace sn
{

f32 SN_API noise2d(s32 x, s32 y, s32 seed);

f32 SN_API noise2dGradient(f32 x, f32 y, s32 seed);

f32 SN_API noise2dPerlin(
    f32 x, f32 y, s32 seed,
    s32 octaves, f32 persistence, f32 period
);

f32 SN_API noise3d(s32 x, s32 y, s32 z, s32 seed);

f32 SN_API noise3dGradient(f32 x, f32 y, f32 z, s32 seed);

f32 SN_API noise3dPerlin(
    f32 x, f32 y, f32 z, s32 seed,
    s32 octaves, f32 persistence, f32 period
);

} // namespace sn


#endif // __HEADER_SN_NOISE__
