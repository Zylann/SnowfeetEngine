#ifndef __HEADER_VOXY_MESHBUILDER__
#define __HEADER_VOXY_MESHBUILDER__

#include "Terrain.h"
#include <core/asset/base/Mesh.hpp>
#include <core/math/Color.hpp>

namespace voxy
{

class MeshBuilder
{
public:
	MeshBuilder();
	~MeshBuilder();

	void process(const Block & block, sn::Mesh & out_mesh);

private:
	enum Direction
	{
		DIR_XP,
		DIR_XN,
		DIR_YP,
		DIR_YN,
		DIR_ZP,
		DIR_ZN
	};

	void makeFace(u8 x, u8 y, u8 z, sn::Color8 c, Direction dir);

private:
	sn::Color8 m_colorPalette[255];
	sn::Mesh * m_mesh;
};

} // namespace voxy

#endif // __HEADER_VOXY_MESHBUILDER__

