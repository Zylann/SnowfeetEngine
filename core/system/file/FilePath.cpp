#include "FilePath.h"
#include <core/util/stringutils.h>

namespace sn
{

String FilePath::join(const String & str1, const String & str2, char separator)
{
    if (str1.empty())
        return str2;
    
    if (str2.empty())
        return str1;

    char a = str1[str1.size() - 1];
    char b = str2[0];

    if (isSeparator(a) ^ isSeparator(b))
    {
        return str1 + str2;
    }
    else
    {
        if (isSeparator(a))
            return str1.substr(0, str1.size()-1) + str2;
        else
            return str1 + (wchar_t)separator + str2;
    }
}

std::string FilePath::join(const std::string & str1, const std::string & str2, char separator)
{
    String res = join(toWideString(str1), toWideString(str2), separator);
    return toString(res);
}

String FilePath::platformize(String str)
{
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == OTHER_SEPARATOR)
            *it = PLATFORM_SEPARATOR;
    }
    return str;
}

std::string FilePath::platformize(std::string str)
{
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == OTHER_SEPARATOR)
            *it = PLATFORM_SEPARATOR;
    }
    return str;
}

String FilePath::normalize(String str)
{
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == PLATFORM_SEPARATOR)
            *it = UNIFIED_SEPARATOR;
    }
    return str;
}

std::string FilePath::normalize(std::string str)
{
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it == PLATFORM_SEPARATOR)
            *it = UNIFIED_SEPARATOR;
    }
    return str;
}

} // namespace sn


