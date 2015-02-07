/*
Color8.cpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <sstream>
#include <core/math/Color.hpp>
#include <core/math/math.hpp>

namespace sn
{

//==============================================================================
// Color
//==============================================================================

Color::Color(const Color8 & c8)
{
    r = static_cast<f32>(c8.r) / 255.0f;
    g = static_cast<f32>(c8.g) / 255.0f;
    b = static_cast<f32>(c8.b) / 255.0f;
    a = static_cast<f32>(c8.a) / 255.0f;
}

//------------------------------------------------------------------------------
void Color::clamp()
{
    r = math::clamp(r, 0.f, 1.f);
    g = math::clamp(g, 0.f, 1.f);
    b = math::clamp(b, 0.f, 1.f);
    a = math::clamp(a, 0.f, 1.f);
}

//------------------------------------------------------------------------------
std::string Color::toString() const
{
    std::stringstream ss;
    ss << "(r=" << r
        << ", g=" << g
        << ", b=" << b
        << ", a=" << a << ")";
    return ss.str();
}

//------------------------------------------------------------------------------
std::string operator+(std::string const & a, Color const & b)
{
    return a + b.toString();
}

//==============================================================================
// Color8
//==============================================================================

Color8::Color8() : r(0), g(0), b(0), a(255)
{}

//------------------------------------------------------------------------------
Color8::Color8(unsigned int hex)
{
    setFromRGBA32(hex);
}

//------------------------------------------------------------------------------
Color8::Color8(u8 r0, u8 g0, u8 b0, u8 a0) : r(r0), g(g0), b(b0), a(a0)
{}

//------------------------------------------------------------------------------
Color8::Color8(const Color8 & other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}

//------------------------------------------------------------------------------
Color8::Color8(const Color & other)
{
    r = static_cast<u8>(other.r * 255.0f);
    g = static_cast<u8>(other.g * 255.0f);
    b = static_cast<u8>(other.b * 255.0f);
    a = static_cast<u8>(other.a * 255.0f);
}

//------------------------------------------------------------------------------
#if defined SN_SFML
Color8::Color8(const sf::Color8 & color)
{
    set(color.r, color.g, color.b, color.a);
}
#endif

//------------------------------------------------------------------------------
#if defined SN_SFML
sf::Color8 Color8::toSfColor() const
{
    return sf::Color8(r, g, b, a);
}
#endif

//------------------------------------------------------------------------------
void Color8::setf(f32 r0, f32 g0, f32 b0, f32 a0)
{
    r = static_cast<u8>(255.f * r0);
    g = static_cast<u8>(255.f * g0);
    b = static_cast<u8>(255.f * b0);
    a = static_cast<u8>(255.f * a0);
}

//------------------------------------------------------------------------------
void Color8::set(u8 r0, u8 g0, u8 b0, u8 a0)
{
    r = r0;
    g = g0;
    b = b0;
    a = a0;
}

//------------------------------------------------------------------------------
void Color8::set(Color8 other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}

//------------------------------------------------------------------------------
void Color8::setFromRGBA32(unsigned int hex)
{
                    a = hex & 0xff;
    hex = hex >> 8;	b = hex & 0xff;
    hex = hex >> 8; g = hex & 0xff;
    hex = hex >> 8; r = hex & 0xff;
}

//------------------------------------------------------------------------------
u32 Color8::asRGBA32() const
{
    u32 hex = r;
    hex = hex << 8; hex |= g;
    hex = hex << 8; hex |= b;
    hex = hex << 8; hex |= a;
    return hex;
}

//------------------------------------------------------------------------------
u16 Color8::asRGBA16() const
{
    u16 oct = r >> 4;
    oct = oct << 8; oct |= g >> 4;
    oct = oct << 8; oct |= b >> 4;
    oct = oct << 8; oct |= a >> 4;
    return oct;
}

//------------------------------------------------------------------------------
void Color8::multiply(f32 s)
{
    r = static_cast<u8>(std::min(r * s, 255.f));
    g = static_cast<u8>(std::min(g * s, 255.f));
    b = static_cast<u8>(std::min(b * s, 255.f));
    a = static_cast<u8>(std::min(a * s, 255.f));
}

//------------------------------------------------------------------------------
void Color8::multiplyRGB(f32 s)
{
    r = static_cast<u8>(std::min(r * s, 255.f));
    g = static_cast<u8>(std::min(g * s, 255.f));
    b = static_cast<u8>(std::min(b * s, 255.f));
}

//------------------------------------------------------------------------------
void Color8::multiplyU8(u8 ku)
{
    const float kf = static_cast<f32>(ku) / 255.f;
    r = (u8)((f32)r * kf);
    g = (u8)((f32)g * kf);
    b = (u8)((f32)b * kf);
    a = (u8)((f32)a * kf);
}

//------------------------------------------------------------------------------
std::string Color8::toString() const
{
    std::stringstream ss;
    ss << "(r=" << (s32)r
      << ", g=" << (s32)g
      << ", b=" << (s32)b
      << ", a=" << (s32)a << ")";
    return ss.str();
}

//------------------------------------------------------------------------------
Color8 Color8::operator+(const Color8 & other) const
{
    return Color8(
        std::min((u32)r + (u32)other.r, 255u),
        std::min((u32)g + (u32)other.g, 255u),
        std::min((u32)b + (u32)other.b, 255u),
        std::min((u32)a + (u32)other.a, 255u));
}

//------------------------------------------------------------------------------
Color8 Color8::operator-(const Color8 & other) const
{
    return Color8(
        r > other.r ? r - other.r : 0,
        g > other.g ? g - other.g : 0,
        b > other.b ? b - other.b : 0,
        a > other.a ? a - other.a : 0);
}

//------------------------------------------------------------------------------
Color8 & Color8::operator+=(const Color8 & other)
{
    r = std::min((u32)r + (u32)other.r, 255u);
    g = std::min((u32)g + (u32)other.g, 255u);
    b = std::min((u32)b + (u32)other.b, 255u);
    a = std::min((u32)a + (u32)other.a, 255u);
    return *this;
}

//------------------------------------------------------------------------------
Color8 & Color8::operator-=(const Color8 & other)
{
    r = r > other.r ? r - other.r : 0;
    r = g > other.g ? g - other.g : 0;
    r = b > other.b ? b - other.b : 0;
    r = a > other.a ? a - other.a : 0;
    return *this;
}

//------------------------------------------------------------------------------
std::string operator+(std::string const & a, Color8 const & b)
{
    return a + b.toString();
}

//------------------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const Color8 & col)
{
    return os << col.toString();
}

} // namespace sn



