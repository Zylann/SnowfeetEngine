/*
interpolation.cpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <core/math/interpolation.hpp>

namespace sn
{
/*
    Linear intepolations
*/

//------------------------------------------------------------------------------
f32 biLinearInterpolationSmooth(
    const f32 x0y0, const f32 x1y0,
    const f32 x0y1, const f32 x1y1,
    const f32 x, const f32 y)
{
    const f32 tx = smoothCurve(x);
    const f32 ty = smoothCurve(y);

    const f32 u = linearInterpolation(x0y0,x1y0, tx);
    const f32 v = linearInterpolation(x0y1,x1y1, tx);

    return linearInterpolation(u,v,ty);
}

//------------------------------------------------------------------------------
f32 triLinearInterpolation(
    const f32 v000, const f32 v100, const f32 v010, const f32 v110,
    const f32 v001, const f32 v101, const f32 v011, const f32 v111,
    const f32 x, const f32 y, const f32 z)
{
    /*double tx = easeCurve(x);
    double ty = easeCurve(y);
    double tz = easeCurve(z);*/
    const f32 tx = x;
    const f32 ty = y;
    const f32 tz = z;

    return(
        v000 * (1 - tx) * (1 - ty) * (1 - tz) +
        v100 * tx * (1 - ty) * (1 - tz) +
        v010 * (1 - tx) * ty * (1 - tz) +
        v110 * tx * ty * (1 - tz) +
        v001 * (1 - tx) * (1 - ty) * tz +
        v101 * tx * (1 - ty) * tz +
        v011 * (1 - tx) * ty * tz +
        v111 * tx * ty * tz
    );
}

/*
    Cubic intepolations
*/

//------------------------------------------------------------------------------
f32 cubicInterpolation(f32 x0, f32 x1, f32 x2, f32 x3, f32 t)
{
    const f32 a = x3 - x2 - x0 + x1;
    const f32 b = x0 - x1 - a;
    const f32 c = x2 - x0;
    const f32 d = x1;

    return a * t*t*t + b * t*t + c * t + d;
}

//------------------------------------------------------------------------------
f32 biCubicInterpolation(f32 x0y[4], f32 x1y[4], f32 x2y[4], f32 x3y[4], f32 x)
{
    const f32 a = cubicInterpolation(x0y[0], x0y[1], x0y[2], x0y[3], x);
    const f32 b = cubicInterpolation(x1y[0], x1y[1], x1y[2], x1y[3], x);
    const f32 c = cubicInterpolation(x2y[0], x2y[1], x2y[2], x2y[3], x);
    const f32 d = cubicInterpolation(x3y[0], x3y[1], x3y[2], x3y[3], x);

    return cubicInterpolation(a,b,c,d, x);
}

} // namespace sn

