#ifndef __HEADER_SN_MATH_FOV__
#define __HEADER_SN_MATH_FOV__

#include <core/math/math.hpp>

namespace sn
{

/// \brief Field of view defined by four half-angle tangent values
struct Fov
{
    /// \brief Tangent of the angle between the left edge of the view and the forward vector
    f32 tanLeft;
    /// \brief Tangent of the angle between the right edge of the view and the forward vector
    f32 tanRight;
    /// \brief Tangent of the angle between the top edge of the view and the forward vector
    f32 tanUp;
    /// \brief Tangent of the angle between the bottom edge of the view and the forward vector
    f32 tanDown;

    /// \brief Creates a Fov from raw tangent values
    Fov(f32 _tanLeft, f32 _tanRight, f32 _tanUp, f32 _tanDown) :
        tanLeft(_tanLeft),
        tanRight(_tanRight),
        tanUp(_tanUp),
        tanDown(_tanDown)
    {}

    /// \brief Creates a Fov from four half-angles in degrees
    static Fov fromDegrees(f32 degreesLeft, f32 degreesRight, f32 degreesUp, f32 degreesDown)
    {
        return {
            tan(degreesLeft * math::DEG2RAD),
            tan(degreesRight * math::DEG2RAD),
            tan(degreesUp * math::DEG2RAD),
            tan(degreesDown * math::DEG2RAD)
        };
    }

    /// \brief Creates a Fov from a full vertical angle and an aspect ratio.
    static Fov fromDegreesRatio(f32 degrees, f32 aspectRatio)
    {
        f32 t = tan(0.5f * degrees * math::DEG2RAD);
        return {
            // Horizontal
            t * aspectRatio, 
            t * aspectRatio, 
            // Vertical
            t, 
            t
        };
    }
};

} // namespace sn

#endif // __HEADER_SN_MATH_FOV__

