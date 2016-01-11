#include <core/util/assert.h>
#include "GUID.h"

namespace sn
{
    namespace
    {
        char * g_hexChars = "0123456789abcdef";

        u8 charToNumber(char c)
        {
            if (c >= '0' && c <= '9')
                return c - '0';
            if (c >= 'a' && c <= 'z')
                return c - 'a' + 10;
            if (c >= 'A' && c <= 'Z')
                return c - 'A' + 10;
            return 0;
        }
    }

std::string GUID::toString(const GUID & guid)
{
    std::string s;
    s.reserve(GUID::BYTE_COUNT * 2);

    const u8 * data = guid.getData();
    for (u32 i = 0; i < GUID::BYTE_COUNT; ++i)
    {
        s += g_hexChars[data[i] & 0x0f];
        s += g_hexChars[(data[i] >> 4) & 0x0f];
    }

    return s;
}

GUID GUID::fromString(const std::string & str)
{
    SN_ASSERT(str.size() == GUID::BYTE_COUNT * 2, "Expected 32-chars GUID string");

    GUID guid;
    for (u32 i = 0; i < GUID::BYTE_COUNT; ++i)
    {
        u32 j = i * 2;
        u8 d = charToNumber(str[j]) | (charToNumber(str[j+1]) << 4);
        guid.m_data[i] = d;
    }

    return guid;
}

} // namespace sn

