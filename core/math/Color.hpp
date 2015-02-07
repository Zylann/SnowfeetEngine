/*
Color.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_COLOR__
#define __HEADER_SN_COLOR__

#include <string>
#include <core/types.hpp>

namespace sn
{

struct Color8;

/// \brief Stores a color using 4 float values for red, green, blue and alpha, in the range [0,1].
/// This version is well-suited for computations.
struct SN_API Color
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(f32 grey) : r(grey), g(grey), b(grey), a(1) {}
    Color(f32 pr, f32 pg, f32 pb, f32 pa = 1.0f) : r(pr), g(pg), b(pb), a(pa) {}
    Color(const Color & other) : r(other.r), g(other.g), b(other.b), a(other.a) {}
    Color(const Color8 & c8);

    void clamp();
    std::string toString() const;
};

std::string operator+(std::string const & a, Color const & b);

/// \brief Stores a color using 4 unsigned byte values for red, green, blue and alpha, in the range [0,255].
/// This version is well-suited for performance and storage.
struct SN_API Color8
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    // Constructs an opaque black color.
    Color8();

    // Constructs a color from an hexadecimal value (RGBA)
    Color8(unsigned int hex);

    // Constructs a color using integer values in [0,255]
    Color8(u8 r0, u8 g0, u8 b0, u8 a0 = 255);

    // Constructs a color from another color
    Color8(const Color8 & other);

    Color8(const Color & other);

    // Assigns [0,1] float values to this color
    void setf(float r0, float g0, float b0, float a0 = 1);

    // Assigns [0,255] integer values to this color
    void set(u8 r0, u8 g0, u8 b0, u8 a0 = 255);

    // Copies values from another color
    void set(Color8 other);

    // Sets the color to an hexadecimal-specified color (RGBA)
    void setFromRGBA32(unsigned int hex);

    // Gets the color as hexadecimal RGBA color
    u32 asRGBA32() const;

    // Gets the color as hexadecimal 16-bit RGBA color
    u16 asRGBA16() const;

    // Converts the object to a human-readable form
    std::string toString() const;

    // Multiplies RGB values by a scalar (alpha is ignored)
    void multiplyRGB(float s);

    // Multiplies RGBA values by a scalar
    void multiply(float s);

    // Multiplies RGBA values by a scalar given in [0,255].
    // It will be treated as a ratio.
    void multiplyU8(u8 ku);

    Color8 operator+(const Color8 & other) const;
    Color8 operator-(const Color8 & other) const;
    Color8 & operator+=(const Color8 & other);
    Color8 & operator-=(const Color8 & other);

    bool operator==(const Color8 & other) const
    {
        return r == other.r
            && g == other.g
            && b == other.b
            && a == other.a;
    }

    // TODO Color: operators *, *=, / and /=
};

std::string operator+(std::string const & a, Color8 const & b);

std::ostream & operator<<(std::ostream & os, const Color8 & col);

} // namespace sn

#endif // __HEADER_SN_COLOR__




