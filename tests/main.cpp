
// Random tests targeting the core are put here.
// In the future this project might be eligible for more advanced testing.

#include <core/math/math.h>

#include <core/math/Vector3.h> // Needed fore NTree<3>
#include <core/space/NTree.h>

#include "tests.hpp"

void testNTree()
{
    sn::NTree<3> octree;
    octree.add(nullptr, sn::FloatAABB(0, 0, 0, 1, 1, 1));
}

int main(int argc, char * argv[])
{
    test_guid();
    //test_sml();
    //test_squirrelBinding();
	//test_variant();
    //test_fileWatcher();
    //test_stringSplit();
    //test_reflection();
    //testNTree();
    //return sn::appMain(argc, argv);

    std::cout << std::endl << "End of test. Press a key to dismiss...";
	getchar();
}



