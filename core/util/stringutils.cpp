/*
stringutils.cpp
Copyright (C) 2010-2012 Marc GILLERON
This file is part of the zCraft project.
*/

#include <sstream>
#include <cmath>

#include <core/util/stringutils.hpp>

namespace sn
{
// TODO stringutils: may be optimized

//------------------------------------------------------------------------------
std::string toString(f32 x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

//------------------------------------------------------------------------------
std::string toString(f64 x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

//------------------------------------------------------------------------------
std::string toString(s32 x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

//------------------------------------------------------------------------------
std::string toString(u32 x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

//------------------------------------------------------------------------------
std::string toString(bool b)
{
    return b ? "true" : "false";
}

//------------------------------------------------------------------------------
std::string toStringBin(u32 x, u8 n)
{
    u32 mask = 1 << (n-1);
    std::string str;

    for(u8 i = 0; i < n; i++, mask = mask >> 1)
    {
        if(x & mask)
            str += "1";
        else
            str += "0";
    }
    return str;
}

//------------------------------------------------------------------------------
void toUpper(std::string & str)
{
    for(u32 i = 0; i < str.size(); i++)
        str[i] = toupper(str[i]);
}

//------------------------------------------------------------------------------
void toLower(std::string & str)
{
    for (u32 i = 0; i < str.size(); i++)
        str[i] = tolower(str[i]);
}

//------------------------------------------------------------------------------
s32 toInt(std::string s)
{
    std::stringstream ss(s);
    s32 i;
    ss >> i;
    return i;
}

//------------------------------------------------------------------------------
std::string toTimeString(f32 s)
{
    s32 min = static_cast<s32>(s) / 60;
    s32 hr = min / 60;
    min = min % 60;
    s32 sec = (static_cast<s32>(s)) % 60;
    s32 ms = static_cast<s32>(1000.0 * (s - floor(s)));

    std::string s_hr;
    std::string s_min;
    std::string s_sec;
    std::string s_ms;

    if(hr == 0)
        s_hr = "00";
    else if(hr < 10)
        s_hr = "0" + toString(hr);
    else
        s_hr = toString(hr);

    if(min == 0)
        s_min = "00";
    else if(min < 10)
        s_min = "0" + toString(min);
    else
        s_min = toString(min);

    if(sec == 0)
        s_sec = "00";
    else if(sec < 10)
        s_sec = "0" + toString(sec);
    else
        s_sec = toString(sec);

    if(ms == 0)
        s_ms = "000";
    else if(ms < 10)
        s_ms = "00" + toString(ms);
    else if(ms < 100)
        s_ms = "0" + toString(ms);
    else
        s_ms = toString(ms);

    return s_hr + ":" + s_min + ":" + s_sec + ":" + s_ms;
}

//------------------------------------------------------------------------------
bool isPrintableChar(u8 c)
{
    return (c >= 32 && c <= 126); // without the char(13) which is '\n'
}

//------------------------------------------------------------------------------
std::string cropStr(std::string str, u8 c)
{
    if(str.empty())
        return str;

    if(str.size() == 1)
    {
        if(str[0] == c)
            return "";
        return str;
    }

    u32 i = str.find(c);
    if(i != std::string::npos)
        str = str.substr(i + 1);

    i = str.find(c);
    if(i != std::string::npos)
        str = str.substr(0, i);

    return str;
}

//------------------------------------------------------------------------------
std::string fileNameWithoutExtension(std::string path)
{
    path = path.substr(0, path.find_first_of("."));
    path = path.substr(path.find_last_of("/\\") + 1);
    return path;
}

//------------------------------------------------------------------------------
std::string toString(const std::wstring & wstr)
{
    std::string str;
    for (u32 i = 0; i < wstr.size(); ++i)
    {
        wchar_t wc = wstr[i];
        str += (s8)wc;
        if (wc & 0xff00)
        {
            str += (s8)(wc >> 8);
        }
        // On Windows, wchar_t is 16 bits wide
#ifndef _WIN32
        if (wc & 0xff0000)
        {
            str += (s8)(wc >> 16);
        }
        if (wc & 0xff000000)
        {
            str += (s8)(wc >> 24);
        }
#endif
    }
    return str;
}

//------------------------------------------------------------------------------
std::wstring toWideString(const std::string & str)
{
    return std::wstring(str.begin(), str.end());
}

//------------------------------------------------------------------------------
std::wstring getFileExtension(const std::wstring & str)
{
    size_t sepIndex = str.find_last_of('.');
    if (sepIndex != std::string::npos)
    {
        return str.substr(sepIndex);
    }
    else
    {
        return L"";
    }
}

//------------------------------------------------------------------------------
std::wstring getFileFolder(const std::wstring & path)
{
    size_t sepIndex = path.find_last_of('/');
    if (sepIndex == std::string::npos)
    {
        sepIndex = path.find_last_of('\\');
    }
    if (sepIndex == std::string::npos)
    {
        return L"";
    }
    else
    {
        return path.substr(0, sepIndex);
    }
}

} // namespace sn


