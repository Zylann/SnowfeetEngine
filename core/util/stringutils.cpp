/*
stringutils.cpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <sstream>
#include <fstream>
#include <cmath>

#include <core/util/stringutils.h>
#include <core/util/Log.h>

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
std::string getFileNameWithoutExtension(std::string path)
{
    path = path.substr(path.find_last_of("/\\") + 1);
    path = path.substr(0, path.find_first_of("."));
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
	// TODO Assume it's UTF8 and convert properly...
    return std::wstring(str.begin(), str.end());
}

//------------------------------------------------------------------------------
std::wstring getFileExtension(const std::wstring & str)
{
    size_t sepIndex = str.find_last_of('.');
    if (sepIndex != std::wstring::npos)
    {
        return str.substr(sepIndex);
    }
    else
    {
        return L"";
    }
}

//------------------------------------------------------------------------------
std::string getFileExtension(const std::string & str)
{
    size_t sepIndex = str.find_last_of('.');
    if (sepIndex != std::string::npos)
    {
        return str.substr(sepIndex);
    }
    else
    {
        return "";
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

//------------------------------------------------------------------------------
std::string trimLeft(std::string str)
{
    u32 i = 0;
    for (; i < str.size(); ++i)
    {
        if (!isspace(str[i]))
            break;
    }
    return str.substr(i);
}

//------------------------------------------------------------------------------
std::string trimRight(std::string str)
{
    u32 i = str.size()-1;
    for (; i < str.size(); --i) // Note: --0 will be UINT_MAX
    {
        if (!isspace(str[i]))
            break;
    }
    return str.substr(0, i+1); // Note: ++UINT_MAX will be 0
}

//------------------------------------------------------------------------------
std::string trim(std::string str)
{
    return trimLeft(trimRight(str));
}

//------------------------------------------------------------------------------
std::vector<std::string> split(const std::string & str, char sep)
{
    std::vector<std::string> strings;

    if (str.empty())
        return strings;

    u32 i_begin = 0;
    for (u32 i = 0; i < str.size(); ++i)
    {
        if (str[i] == sep)
        {
            strings.push_back(str.substr(i_begin, i - i_begin));
            i_begin = i + 1; // +1 to skip the separator
        }
    }

    if (i_begin < str.size())
    {
        strings.push_back(str.substr(i_begin));
    }

    return strings;
}

//------------------------------------------------------------------------------
bool readFile(const std::string & filePath, std::string & str)
{
    std::ifstream ifs(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!ifs.good())
    {
        SN_ERROR("Couldn't open file \"" + filePath + '"');
        return false;
    }

    ifs.seekg(0, std::ios::end);
    std::streamoff len = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    str.resize(len, '\0');

    ifs.read(&str[0], len);

    return true;
}

} // namespace sn


