#ifndef __HEADER_SN_GUID_WIN32__
#define __HEADER_SN_GUID_WIN32__

#include <core/util/assert.h>
#include "../GUID.h"
#include <Objbase.h>

namespace sn
{

GUID GUID::create()
{
    // Get GUID from the system

    ::GUID sysGuid;
    HRESULT res = CoCreateGuid(&sysGuid);

    SN_STATIC_ASSERT_MSG(sizeof(::GUID) == sn::GUID::BYTE_COUNT, "System and engine GUID sizes differ");
    
    // Copy GUID to the engine's struct (which is normalized)
    // https://en.wikipedia.org/wiki/Globally_unique_identifier

    sn::GUID guid;
    u8 * data = guid.m_data;

    data[0] = (sysGuid.Data1 >> 24) & 0xff;
    data[1] = (sysGuid.Data1 >> 16) & 0xff;
    data[2] = (sysGuid.Data1 >> 8) & 0xff;
    data[3] = (sysGuid.Data1) & 0xff,

    data[4] = (sysGuid.Data2 >> 8) & 0xff;
    data[5] = (sysGuid.Data2) & 0xff;

    data[6] = (sysGuid.Data3 >> 8) & 0xff;
    data[7] = (sysGuid.Data3) & 0xff;

    for (u32 i = 0; i < 8; ++i)
        data[i + 8] = sysGuid.Data4[i];

    return guid;
}

} // namespace sn

#endif // __HEADER_SN_GUID_WIN32__

