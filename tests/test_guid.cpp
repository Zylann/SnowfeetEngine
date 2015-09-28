#include "tests.hpp"
#include <core/system/GUID.h>
#include <core/util/Log.h>
#include <iostream>

using namespace sn;

void test_guid()
{
    GUID guid = GUID::create();

    std::string guidStr = toString(guid);

    SN_LOG("First GUID:     " << guidStr);

    GUID guid2 = GUID::fromString(guidStr);

    SN_LOG("After reparse:  " << toString(guid2));

    SN_LOG("Series of GUIDs:");

    for (u32 i = 0; i < 10; ++i)
    {
        SN_LOG(toString(GUID::create()));
    }
}
