#ifndef __HEADER_VOXY_MESHBUILDER__
#define __HEADER_VOXY_MESHBUILDER__

#include "Terrain.h"
#include <core/math/Color.h>
#include <modules/render/Mesh.h>

namespace voxy
{

class MeshBuilder
{
public:
	void process(const Block & block, sn::Mesh & out_mesh);

private:
	sn::Color8 m_colorPalette[255];

};

} // namespace voxy

#endif // __HEADER_VOXY_MESHBUILDER__

