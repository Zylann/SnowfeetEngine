/*
Matrix4.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MATRIX4__
#define __HEADER_SN_MATRIX4__

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Fov.hpp"

namespace sn
{

/// \brief A 4x4 float matrix.
/// Values are stored in row-major layout.
/// Translations are stored in the last row.
class SN_API Matrix4
{
public :

	/// \brief Constructs a matrix initialized to identity
    Matrix4();

	/// \brief Constructs a matrix which is a copy of another
    Matrix4(const Matrix4 & other);

	/// \brief Constructs a matrix from raw values
    Matrix4(const f32 values[16]);

	/// \brief Copies values from another matrix
    void set(const Matrix4 & other);

	/// \brief Copies raw values into the matrix
    void set(const f32 values[16]);

	/// \brief Gets the one-dimensional index of one cell's coordinates
	/// \param row: row
	/// \param col: column
    inline u32 getCellIndex(u32 row, u32 col) const { return col | (row * 4); }

	/// \brief Gets a cell's value from row and column
    inline f32 Matrix4::getCell(u32 row, u32 col) const { return m_v[getCellIndex(row, col)]; }

	/// \brief Sets a cell's value from row and column
    inline void Matrix4::setCell(u32 row, u32 col, f32 v) { m_v[getCellIndex(row, col)] = v; }

    /// \brief Sets the matrix to identity
    void loadIdentity();

    /// \brief Sets the matrix to a 3D perspective projection matrix with vertical field of view and aspect ratio
    void loadPerspectiveProjection(
        const f32 fov, const f32 ratio,
        const f32 near, const f32 far
    );

	/// \brief Sets the matrix to a 3D perspective projection matrix with general field of view
    void loadPerspectiveProjection(Fov fov, f32 near, f32 far);

	/// \brief Sets the matrix to an orthographic projection matrix
    void loadOrtho2DProjection(
        const f32 left, const f32 top,
        const f32 right, const f32 bottom,
        const f32 near, const f32 far
    );

	/// \brief Sets the matrix to a lookat matrix for use in 3D cameras.
	/// \param eye: position of the camera
	/// \param target: position to look at
	/// \param up: normalized up vector
    void loadLookAt(
        const sn::Vector3f & eye,
        const sn::Vector3f & target,
        const sn::Vector3f & up
    );

    /// \brief Sets the matrix to a translation matrix.
	/// \param vx: translation X delta
	/// \param vy: translation Y delta
	/// \param vz: translation Z delta
    void loadTranslation(const f32 vx, const f32 vy, const f32 vz);

    /// \brief Sets the matrix to a rotation matrix.
    /// The rotation is defined from an angle around an axis.
	/// \param t: angle around the axis, in radians
	/// \param x: normalized X coordinate of the axis
	/// \param y: normalized Y coordinate of the axis
	/// \param z: normalized Z coordinate of the axis
    void loadRotation(const f32 t, const f32 x, const f32 y, const f32 z);

    /// \brief Sets the matrix to a scaling matrix
    void loadScale(const f32 sx, const f32 sy, const f32 sz);

    /// \brief Sets the matrix to the result of the product of the given matrices
    void setByProduct(const Matrix4 & in_a, const Matrix4 & in_b);

    /// \brief Sets the matrix to the result of the product of the given matrices, as if they were 3x3.
    void setByProduct3x3(const Matrix4 & a, const Matrix4 & b);

	/// \brief Gets the transposed version of this matrix into another
	/// \param out_result: matrix that will be filled with the result
    void getTransposed(Matrix4 & out_result) const;

	/// \brief Transposes the matrix.
    void transpose();

	/// \brief Transposes the first 3 rows and columns of the matrix (as if it was 3x3)
    void transpose3x3();

    /// \brief Sets the translation part of the matrix
    void setTranslation(const Vector3f & v);

    /// \brief Sets the rotation part of the matrix.
    void setRotation(const Quaternion & q);

    /// \brief Scales the transformation represented by the matrix by a factor.
    void scaleTransform(const Vector3f & s);

    /// \brief Gets the raw values of the matrix as a one-dimensional array.
    /// \return array of 16 float values
    inline const f32 * values() const { return m_v; }

    /// \brief Calculates and returns the determinant of the matrix.
    f32 getD() const;

    /// \brief Tries to compute the inverse matrix.
    /// \param out_result: result of the inversion. Not modified if it fails.
    /// \return true if the matrix could be inverted, false otherwise.
    bool getInverse(Matrix4 & out_result) const;

    /// \brief Applies the transformation represented by the matrix to a 3D point.
    /// \param p: point to transform
    /// \return transformed point
	Vector3f transformPoint(const Vector3f & p) const;

    //-------------------------------------
    // Operators
    //-------------------------------------

    void operator=(const Matrix4 & other);
    inline f32 & operator()(const u32 row, const u32 col) { return m_v[getCellIndex(row, col)]; }
    inline f32 operator()(const u32 row, const u32 col) const { return m_v[getCellIndex(row, col)]; }

private :

    inline f32 operator[](s32 i) const { return m_v[i]; }

    /// \brief Cell values.
    ///
    ///  0   1   2   3 | i(x, y, z) right
    ///  4   5   6   7 | j(x, y, z) up
    ///  8   9  10  11 | k(x, y, z) front
    /// 12  13  14  15 | t(x, y, z) offset
    ///
    f32 m_v[16];
};

} // namespace sn

#endif // __HEADER_SN_MATRIX4__

