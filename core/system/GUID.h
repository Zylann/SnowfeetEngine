#ifndef __HEADER_SN_GUID__
#define __HEADER_SN_GUID__

#include <core/types.h>
#include <string>

namespace sn
{

/// \brief Globally Unique IDentifier.
/// Useful to identify objects across distributed systems without relying on a global state.
class SN_API GUID
{
public:
    static const u32 BYTE_COUNT = 16;

    /// \brief Creates a new GUID.
    /// There is practically no chance that calling this function twice produces the same GUID.
    /// \note This function is system-specific.
    static GUID create();

    /// \brief Writes a serializable representation of the GUID in a string.
    /// The used format is a 32-chars string without dashes.
    static std::string toString(const GUID & guid);

    /// \brief Parses a string into a GUID.
    /// The expected format is the same as toString().
    static GUID fromString(const std::string & str);

    GUID()
    {
        for (u32 i = 0; i < BYTE_COUNT; ++i)
            m_data[i] = 0;
    }

    GUID(const GUID & other)
    {
        *this = other;
    }

    GUID & operator=(const GUID & other)
    {
        for (u32 i = 0; i < BYTE_COUNT; ++i)
            m_data[i] = other.m_data[i];
        return *this;
    }

    bool operator==(const GUID & other) const
    {
        for (u32 i = 0; i < BYTE_COUNT; ++i)
        {
            if (m_data[i] != other.m_data[i])
                return false;
        }
        return true;
    }

    bool operator!=(const GUID & other) const
    {
        for (u32 i = 0; i < BYTE_COUNT; ++i)
        {
            if (m_data[i] != other.m_data[i])
                return true;
        }
        return false;
    }

    const u8 * getData() const
    {
        return m_data;
    }

private:
    u8 m_data[BYTE_COUNT];

};

inline std::string toString(const GUID & guid)
{
    return GUID::toString(guid);
}

} // namespace sn

#endif // __HEADER_SN_GUID__

