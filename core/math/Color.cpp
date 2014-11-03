/*
Color.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#include <sstream>
#include <core/math/Color.hpp>
#include <core/math/math.hpp>

namespace sn
{

Color::Color() : r(0), g(0), b(0), a(255)
{}

//------------------------------------------------------------------------------
Color::Color(unsigned int hex)
{
    setFromRGBA32(hex);
}

//------------------------------------------------------------------------------
Color::Color(u8 r0, u8 g0, u8 b0, u8 a0) : r(r0), g(g0), b(b0), a(a0)
{}

//------------------------------------------------------------------------------
Color::Color(const Color & other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}

//------------------------------------------------------------------------------
#if defined ZN_SFML
Color::Color(const sf::Color & color)
{
    set(color.r, color.g, color.b, color.a);
}
#endif

//------------------------------------------------------------------------------
#if defined ZN_SFML
sf::Color Color::toSfColor() const
{
    return sf::Color(r, g, b, a);
}
#endif

//------------------------------------------------------------------------------
void Color::setf(f32 r0, f32 g0, f32 b0, f32 a0)
{
    r = static_cast<u8>(255.f * r0);
    g = static_cast<u8>(255.f * g0);
    b = static_cast<u8>(255.f * b0);
    a = static_cast<u8>(255.f * a0);
}

//------------------------------------------------------------------------------
void Color::set(u8 r0, u8 g0, u8 b0, u8 a0)
{
    r = r0;
    g = g0;
    b = b0;
    a = a0;
}

//------------------------------------------------------------------------------
void Color::set(Color other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}

//------------------------------------------------------------------------------
void Color::setFromRGBA32(unsigned int hex)
{
                    a = hex & 0xff;
    hex = hex >> 8;	b = hex & 0xff;
    hex = hex >> 8; g = hex & 0xff;
    hex = hex >> 8; r = hex & 0xff;
}

//------------------------------------------------------------------------------
u32 Color::asRGBA32() const
{
    u32 hex = r;
    hex = hex << 8; hex |= g;
    hex = hex << 8; hex |= b;
    hex = hex << 8; hex |= a;
    return hex;
}

//------------------------------------------------------------------------------
u16 Color::asRGBA16() const
{
    u16 oct = r >> 4;
    oct = oct << 8; oct |= g >> 4;
    oct = oct << 8; oct |= b >> 4;
    oct = oct << 8; oct |= a >> 4;
    return oct;
}

//------------------------------------------------------------------------------
void Color::multiply(f32 s)
{
    r = static_cast<u8>(math::min(r * s, 255.f));
    g = static_cast<u8>(math::min(g * s, 255.f));
    b = static_cast<u8>(math::min(b * s, 255.f));
    a = static_cast<u8>(math::min(a * s, 255.f));
}

//------------------------------------------------------------------------------
void Color::multiplyRGB(f32 s)
{
    r = static_cast<u8>(math::min(r * s, 255.f));
    g = static_cast<u8>(math::min(g * s, 255.f));
    b = static_cast<u8>(math::min(b * s, 255.f));
}

//------------------------------------------------------------------------------
void Color::multiplyU8(u8 ku)
{
    const float kf = static_cast<f32>(ku) / 255.f;
    r = (u8)((f32)r * kf);
    g = (u8)((f32)g * kf);
    b = (u8)((f32)b * kf);
    a = (u8)((f32)a * kf);
}

//------------------------------------------------------------------------------
std::string Color::toString() const
{
    std::stringstream ss;
    ss << "(R=" << (s32)r
      << ", G=" << (s32)g
      << ", B=" << (s32)b
      << ", A=" << (s32)a << ")";
    return ss.str();
}

//------------------------------------------------------------------------------
Color Color::operator+(const Color & other)
{
    return Color(
        math::min((u32)r + (u32)other.r, 255u),
        math::min((u32)g + (u32)other.g, 255u),
        math::min((u32)b + (u32)other.b, 255u),
        math::min((u32)a + (u32)other.a, 255u));
}

//------------------------------------------------------------------------------
Color Color::operator-(const Color & other)
{
    return Color(
        r > other.r ? r - other.r : 0,
        g > other.g ? g - other.g : 0,
        b > other.b ? b - other.b : 0,
        a > other.a ? a - other.a : 0);
}

//------------------------------------------------------------------------------
void Color::operator+=(const Color & other)
{
    r = math::min((u32)r + (u32)other.r, 255u);
    g = math::min((u32)g + (u32)other.g, 255u);
    b = math::min((u32)b + (u32)other.b, 255u);
    a = math::min((u32)a + (u32)other.a, 255u);
}

//------------------------------------------------------------------------------
void Color::operator-=(const Color & other)
{
    r = r > other.r ? r - other.r : 0;
    r = g > other.g ? g - other.g : 0;
    r = b > other.b ? b - other.b : 0;
    r = a > other.a ? a - other.a : 0;
}

//------------------------------------------------------------------------------
std::string operator+(std::string const & a, Color const & b)
{
    return a + b.toString();
}

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Color & col)
{
    return os << col.toString();
}

} // namespace sn



