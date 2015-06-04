#ifndef __HEADER_VOXY_TERRAIN__
#define __HEADER_VOXY_TERRAIN__

#include "VoxelArray.h"
#include <unordered_map>
#include <core/math/Vector3.hpp>

namespace voxy
{

typedef VoxelArray<16,16,16> Block;

class Terrain
{
public:
	

private:
	std::unordered_map<sn::Vector3i, Block*> m_blocks;
};

} // namespace voxy

#endif // __HEADER_VOXY_TERRAIN__

