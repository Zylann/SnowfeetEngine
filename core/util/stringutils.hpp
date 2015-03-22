/*
stringutils.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_STRINGUTILS__
#define __HEADER_SN_STRINGUTILS__

#include <string>
#include <vector>
#include <core/types.hpp>

namespace sn
{
// TODO stringutils: use std::to_string and make standard functions
// But MinGW doesn't seems to include it yet...

std::string SN_API toString(f32 x);
std::string SN_API toString(f64 x);
std::string SN_API toString(s32 x);
std::string SN_API toString(u32 x);
std::string SN_API toString(bool b);

std::string SN_API toStringBin(u32 x, u8 n=32);
std::string SN_API toStringAlnum(s32 x);

s32 SN_API toInt(std::string s);
void SN_API toUpper(std::string & str);
void SN_API toLower(std::string & str);

// convert a time given in seconds to a time string formated like "hh:mm:ss:ms"
std::string SN_API toTimeString(f32 s);

bool SN_API isPrintableChar(u8 c);

// TODO Move file related functions to core/system/file
std::string SN_API getFileNameWithoutExtension(std::string path);

std::string SN_API toString(const std::wstring & wstr);
std::wstring SN_API toWideString(const std::string & str);

/// \brief Gets the file extension within the given path, if any (the dot is included).
std::string SN_API getFileExtension(const std::string & str);

/// \brief Gets the file extension within the given path, if any (the dot is included).
std::wstring SN_API getFileExtension(const std::wstring & str);

// Returns the folder in which the given file is.
// If the path points to a folder, the parent folder is returned.
// If the path ends with a '/' or a '\', the function just removes it.
std::wstring SN_API getFileFolder(const std::wstring & path);

/// \brief Returns a substring without trailing whitespace.
std::string SN_API trimLeft(std::string str);

/// \brief Returns a substring without leading whitespace. 
std::string SN_API trimRight(std::string str);

/// \brief Returns a substring without surrounding whitespace.
std::string SN_API trim(std::string str);

/// \brief Splits the given string at sep and returns the list without sep
std::vector<std::string> SN_API split(const std::string & str, char sep);

} // namespace sn

#endif // __HEADER_SN_STRINGUTILS__


