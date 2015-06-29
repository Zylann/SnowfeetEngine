#include "tests.hpp"

#include <core/math/Vector3.h>
#include <set>

void test_hashes()
{
    std::set<size_t> hashes;
    std::set<sn::Vector3i> vecs;
    unsigned int n = 0;
    unsigned int collisions = 0;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (unsigned int j = 0; j < 100; ++j)
    {
        for (unsigned int i = 0; i < 10000; ++i)
        {
            sn::Vector3i v(rand(), rand(), rand());
            if (vecs.find(v) == vecs.end())
            {
                vecs.insert(v);
                size_t h = sn::getHash(v);
                auto it = hashes.find(h);
                if (it != hashes.end())
                    ++collisions;
                else
                    hashes.insert(h);
                ++n;
            }
        }
        std::cout << "Iterations: " << n << ", collisions: " << collisions << std::endl;
    }
}

