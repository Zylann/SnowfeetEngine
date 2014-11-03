﻿/*
Matrix4.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MATRIX4__
#define __HEADER_SN_MATRIX4__

#include "Vector3.hpp"

namespace sn
{

/// \brief A 4x4 float matrix.
class SN_API Matrix4
{
public :

    Matrix4();
    Matrix4(const Matrix4 & other);
    Matrix4(const f32 values[16]);

    void set(const Matrix4 & other);
    void set(const f32 values[16]);

    inline f32 Matrix4::getCell(u32 row, u32 col) const { return m_v[col | row << 2]; }
    inline void Matrix4::setCell(u32 row, u32 col, f32 v) { m_v[col | row << 2] = v; }

    // Sets the matrix to identity
    void loadIdentity();

    // Sets the matrix to a 3D perspective projection matrix
    void loadPerspectiveProjection(
        const f32 fov, const f32 ratio,
        const f32 near, const f32 far
    );

    void loadOrtho2DProjection(
        const f32 left, const f32 top,
        const f32 right, const f32 bottom
    );

    void loadLookAt(
        const sn::Vector3f & eye,
        const sn::Vector3f & target,
        const sn::Vector3f & up
    );

    // Sets the matrix to a translation matrix.
    void loadTranslation(const f32 vx, const f32 vy, const f32 vz);

    // Sets the matrix to a rotation matrix.
    // The rotation is defined from an angle around an axis.
    void loadRotation(const f32 t, const f32 x, const f32 y, const f32 z);

    // Sets the matrix to a scaling matrix
    void loadScale(const f32 sx, const f32 sy, const f32 sz);

    // Sets the matrix to the result of the product of the given matrices
    void setByProduct(const Matrix4 & a, const Matrix4 & b);

    // Sets the matrix to the result of the product of the given matrices,
    // as if they were 3x3.
    void setByProductAs3(const Matrix4 & a, const Matrix4 & b);

    void getTransposed(Matrix4 & out_result) const;

    inline const f32 * values() const { return m_v; }

    f32 getD() const;

    bool getInverse(Matrix4 & out_result) const;

    void operator=(const Matrix4 & other);
    inline f32 & operator()(const u32 row, const u32 col) { return m_v[col | row << 2]; }
    inline f32 operator()(const u32 row, const u32 col) const { return m_v[col | row << 2]; }

private :

    inline f32 operator[](s32 i) const { return m_v[i]; }

    //  0   1   2   3
    //  4   5   6   7
    //  8   9  10  11
    // 12  13  14  15
    f32 m_v[16];
};

} // namespace sn

#endif // __HEADER_SN_MATRIX4__

