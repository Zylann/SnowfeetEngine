#include "FilePath.hpp"

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

String FilePath::platformize(String str)
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

} // namespace sn


