#include "MeshBuilder.h"

using namespace sn;

namespace voxy
{

	namespace
	{
		struct Face
		{

		};

		struct VoxelDescription
		{
			bool isTransparent;
			bool isCube;
		};


	}

void MeshBuilder::process(const Block & block, sn::Mesh & out_mesh)
{
	//const u8 * attribArray = block.getArray(Block::ATTRIB_COLOR);

	for (u32 z = 0; z < block.sizeZ(); ++z)
	{
		for (u32 y = 0; y < block.sizeY(); ++y)
		{
			u8 a_prev = 0;
			for (u32 x = 0; x < block.sizeX(); ++x)
			{
				u8 a = block.getValueRaw(ATTRIB_COLOR, block.getIndex(x, y, z));
				
                // TODO


				a_prev = a;
			}
		}
	}
}

} // namespace voxy

