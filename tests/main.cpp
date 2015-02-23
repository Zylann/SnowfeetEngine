
// Random tests targeting the core are put here.
// In the future this project might be eligible for more advanced testing.

#include <core/app/appmain.hpp>

#include <core/math/Vector3.hpp>
#include <core/math/math.hpp>
#include <map>
#include <set>


void testHashes()
{
    std::set<size_t> hashes;
    std::set<sn::Vector3i> vecs;
    unsigned int n = 0;
    unsigned int collisions = 0;

    srand(time(NULL));

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

int main(int argc, char * argv[])
{
    testHashes();
    //return sn::appMain(argc, argv);
}



