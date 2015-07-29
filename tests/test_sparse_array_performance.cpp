#include "tests.hpp"

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>

#include <core/types.h>
#include <core/math/math.h>
#include <core/system/Clock.h>


void test_sparseArrayPerformance()
{
    using namespace sn;

    std::vector<int> vec;
    std::unordered_map<u32, int> umap;
    std::map<u32, int> map;

    const u32 max = 1000;

    for (size_t i = 0; i < 100; ++i)
    {
        u32 k = math::rand(0, max);

        map[k] = 1;

        umap[k] = 1;

        if (vec.size() < k)
            vec.resize(k + 1);
        vec[k] = 1;
    }

    const u32 iterations = 100000;
    Time mapTime;
    Time umapTime;
    Time vectorTime;
    Clock clock;

    // map
    for (size_t i = 0; i < iterations; ++i)
    {
        int n = map[math::rand(0,max)];
    }

    mapTime = clock.restart();

    // umap
    for (size_t i = 0; i < iterations; ++i)
    {
        int n = umap[math::rand(0,max)];
    }

    umapTime = clock.restart();

    // vector
    for (size_t i = 0; i < iterations; ++i)
    {
        u32 k = math::rand(0, max);
        if (k >= vec.size())
            continue;
        int n = vec[k];
    }

    vectorTime = clock.restart();

    std::cout << "mapTime:     " << mapTime.asMilliseconds() << std::endl;
    std::cout << "umapTime:    " << umapTime.asMilliseconds() << std::endl;
    std::cout << "vectorTime:  " << vectorTime.asMilliseconds() << std::endl;
}

