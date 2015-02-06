/*
noise.cpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <cmath>
#include <iostream>

#include <core/math/noise.hpp>
#include <core/math/interpolation.hpp>

// set at random
#define RAND_SEQ_X 72699
#define RAND_SEQ_Y 31976
#define RAND_SEQ_Z 16863
#define RAND_SEQ_SEED 561
#define RAND_SEQ1 11126
#define RAND_SEQ2 98756
#define RAND_SEQ3 423005601
// constant
#define MAX_INT7 0x7fffffff

namespace sn
{
/*
    These functions generate noise from a seed and coordinates.
    Using the same seed and coordinates will generate the same noise.
*/

//------------------------------------------------------------------------------
// [0,1]
f32 noise2d(s32 x, s32 y, s32 seed)
{
     // "hazard" from known random sequences, seed and coordinates
    s32 n = RAND_SEQ_X*x + RAND_SEQ_Y*y + RAND_SEQ_SEED*seed;
    n &= 0x7fffffff; // clear last sign bit
    n = (n >> 13) ^ n;
    n = n * (n * n * RAND_SEQ1 + RAND_SEQ2) + RAND_SEQ3;
    n &= 0x7fffffff;
    // returning [0,1] f32
    return (f32)n / MAX_INT7;
}

//------------------------------------------------------------------------------
f32 noise2dGradient(f32 x, f32 y, s32 seed)
{
    // Calculate the integer coordinates
    s32 x0 = (x > 0.0 ? (s32)x : (s32)x - 1);
    s32 y0 = (y > 0.0 ? (s32)y : (s32)y - 1);
    // Calculate the remaining part of the coordinates
    f32 xl = x - (f32)x0;
    f32 yl = y - (f32)y0;
    // Get values for corners of square
    f32 v00 = noise2d(x0, y0, seed);
    f32 v10 = noise2d(x0+1, y0, seed);
    f32 v01 = noise2d(x0, y0+1, seed);
    f32 v11 = noise2d(x0+1, y0+1, seed);
    // Interpolate
    return biLinearInterpolationSmooth(v00,v10,v01,v11,xl,yl);
}

//------------------------------------------------------------------------------
f32 noise2dPerlin(
        f32 x, f32 y, s32 seed,
        s32 octaves, f32 persistence, f32 period)
{
    if(octaves < 1)
        return 0;

    x /= period;
    y /= period;

    f32 noise = 0; // noise
    f32 f = 1.0;
    f32 amp = 1.0; // amplitude of an octave
    f32 ampMax = 0; // total amplitude

    for(s32 i = 0; i < octaves; i++)
    {
        noise += amp * noise2dGradient(x*f, y*f, seed+i);
        ampMax += amp;
        f *= 2.0;
        amp *= persistence; // reduce next amplitude
    }

    return noise / ampMax;
}

//------------------------------------------------------------------------------
// [0,1]
f32 noise3d(s32 x, s32 y, s32 z, s32 seed)
{
     // "hazard" from known random sequences, seed and coordinates
    s32 n = RAND_SEQ_X*x + RAND_SEQ_Y*y + RAND_SEQ_Z*z + RAND_SEQ_SEED*seed;
    n &= 0x7fffffff; // clear last sign bit
    n = (n >> 13) ^ n;
    n = n * (n * n * RAND_SEQ1 + RAND_SEQ2) + RAND_SEQ3;
    n &= 0x7fffffff;
    // returning [0,1] f32
    return (f32)n / MAX_INT7;
}

//------------------------------------------------------------------------------
f32 noise3dGradient(f32 x, f32 y, f32 z, s32 seed)
{
    // Calculate the integer coordinates
    s32 x0 = (x > 0.0 ? (s32)x : (s32)x - 1);
    s32 y0 = (y > 0.0 ? (s32)y : (s32)y - 1);
    s32 z0 = (z > 0.0 ? (s32)z : (s32)z - 1);
    // Calculate the remaining part of the coordinates
    f32 xl = x - (f32)x0;
    f32 yl = y - (f32)y0;
    f32 zl = z - (f32)z0;
    // Get values for corners of cube
    f32 v000 = noise3d(x0, y0, z0, seed);
    f32 v100 = noise3d(x0+1, y0, z0, seed);
    f32 v010 = noise3d(x0, y0+1, z0, seed);
    f32 v110 = noise3d(x0+1, y0+1, z0, seed);
    f32 v001 = noise3d(x0, y0, z0+1, seed);
    f32 v101 = noise3d(x0+1, y0, z0+1, seed);
    f32 v011 = noise3d(x0, y0+1, z0+1, seed);
    f32 v111 = noise3d(x0+1, y0+1, z0+1, seed);
    // Interpolate
    return triLinearInterpolation(v000,v100,v010,v110,v001,v101,v011,v111,xl,yl,zl);
}

//------------------------------------------------------------------------------
f32 noise3dPerlin(
        f32 x, f32 y, f32 z, s32 seed,
        s32 octaves, f32 persistence, f32 period)
{
    if(octaves < 1)
        return 0;

    x /= period;
    y /= period;
    z /= period;

    f32 noise = 0;
    f32 f = 1.0;
    f32 amp = 1.0;
    f32 ampMax = 0;

    for(s32 i=0; i<octaves; i++)
    {
        noise += amp * noise3dGradient(x*f, y*f, z*f, seed+i);
        ampMax += amp;
        f *= 2.0;
        amp *= persistence;
    }
    return noise / ampMax;
}

} // namespace sn



