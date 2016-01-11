#ifndef __HEADER_SN_FILEPATH__
#define __HEADER_SN_FILEPATH__

#include <core/util/String.h>

namespace sn
{

class SN_API FilePath
{
public:

#ifdef _WIN32
    static const char PLATFORM_SEPARATOR = '\\';
    static const char OTHER_SEPARATOR = '/';
#else
    static const char PLATFORM_SEPARATOR = '/';
    static const char OTHER_SEPARATOR = '\\';
#endif
    static const char UNIFIED_SEPARATOR = '/';

    // Note: the following functions allow working with strings.
    // In the future, FilePath may be usable as an instance too.

    // TODO Use UTF8 everywhere!

    /// \brief Combines two normalized paths and ensures they are well separated with the appropriate symbol
    static String join(const String & str1, const String & str2, char separator=UNIFIED_SEPARATOR);
    static std::string join(const std::string & str1, const std::string & str2, char separator=UNIFIED_SEPARATOR);

    /// \brief Converts a path to the current platform's format
    static String platformize(String str);
    static std::string platformize(std::string str);

    /// \brief Converts a path to the engine's convention
    static String normalize(String str);
    static std::string normalize(std::string str);

    static bool isSeparator(char c) { return c == PLATFORM_SEPARATOR || c == OTHER_SEPARATOR; }

private:
    // This class is not available yet in instance form
    FilePath() {}

    //void append(const String & str);
    //String getPlatformized();
    //String getNormalized();
};

} // namespace sn

#endif __HEADER_SN_FILEPATH__

