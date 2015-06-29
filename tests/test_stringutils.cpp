#include "tests.hpp"

#include <core/util/stringutils.h>
#include <core/math/Vector3.h>

void printVector(const std::vector<std::string> & vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        SN_LOG("'" << *it << "'");
    }
}

void test_stringSplit()
{
    printVector(sn::split("", '.')); SN_LOG("--------");
    printVector(sn::split(".", '.')); SN_LOG("--------");
    printVector(sn::split("..", '.')); SN_LOG("--------");
    printVector(sn::split(".hello.", '.')); SN_LOG("--------");
    printVector(sn::split("hello.world", '.')); SN_LOG("--------");
    printVector(sn::split("hello.my.world", '.')); SN_LOG("--------");
    printVector(sn::split("..hello.world", '.')); SN_LOG("--------");
    printVector(sn::split("hello", '.')); SN_LOG("--------");
}



