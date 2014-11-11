/*
Color.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_COLOR__
#define __HEADER_SN_COLOR__

#include <string>
#include <core/types.hpp>

#ifdef SN_SFML
    #include <SFML/Graphics.hpp>
#endif

namespace sn
{

/*
    Stores a color using 4 unsigned byte values for red, green, blue and alpha.
    This version is well-suited for performance and storage.
*/
struct SN_API Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    // Constructs an opaque black color.
    Color();

    // Constructs a color from an hexadecimal value (RGBA)
    Color(unsigned int hex);

    // Constructs a color using integer values in [0,255]
    Color(u8 r0, u8 g0, u8 b0, u8 a0 = 255);

    // Constructs a color from another color
    Color(const Color & other);

#ifdef SN_SFML
    // Constructs a color from a SFML color
    Color(const sf::Color & color);
#endif

#ifdef SN_SFML
    // Converts this color to a SFML color
    sf::Color toSfColor() const;
#endif

    // Assigns [0,1] float values to this color
    void setf(float r0, float g0, float b0, float a0 = 1);

    // Assigns [0,255] integer values to this color
    void set(u8 r0, u8 g0, u8 b0, u8 a0 = 255);

    // Copies values from another color
    void set(Color other);

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

    Color operator+(const Color & other) const;
    Color operator-(const Color & other) const;
    Color & operator+=(const Color & other);
    Color & operator-=(const Color & other);

    bool operator==(const Color & other) const
    {
        return r == other.r
            && g == other.g
            && b == other.b
            && a == other.a;
    }

    // TODO Color: operators *, *=, / and /=
};

std::string operator+(std::string const & a, Color const & b);

std::ostream & operator<<(std::ostream & os, const Color & col);

//#ifdef SN_SFML
//	sf::Color & operator=(sf::Color & sfcolor, const Color & color);
//#endif

} // namespace sn

#endif // __HEADER_SN_COLOR__




