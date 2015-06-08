#include "MeshBuilder.h"
#include <core/util/Profiler.h>

using namespace sn;

namespace voxy
{

	namespace
	{
		struct VoxelDescription
		{
			bool isTransparent;
			bool isCube;
		};

		inline bool isOpaque(const Color8 c)
		{
			return c.a == 255;
		}
	}

MeshBuilder::MeshBuilder()
{
	m_mesh = new Mesh();
	m_mesh->setPrimitiveType(SN_MESH_QUADS);
}

MeshBuilder::~MeshBuilder()
{
	delete m_mesh;
}

void MeshBuilder::process(const Block & block, sn::Mesh & out_mesh)
{
	SN_BEGIN_PROFILE_SAMPLE_NAMED("Voxy MeshBuilder::process");

	m_mesh->clear();

	const int pad = 1;

	// Make cubes
	for (u32 z = pad; z < block.sizeZ() - pad; ++z)
	{
		for (u32 y = pad; y < block.sizeY() - pad; ++y)
		{
			for (u32 x = pad; x < block.sizeX() - pad; ++x)
			{
				u8 a = block.getValueRaw(ATTRIB_COLOR, block.getIndex(x, y, z));
				Color8 c = m_colorPalette[a];
				
				if (c.a != 0)
				{
					Color8 c_left =   m_colorPalette[block.getValueRaw(ATTRIB_COLOR, block.getIndex( x-1, y,   z   ))];
					Color8 c_right =  m_colorPalette[block.getValueRaw(ATTRIB_COLOR, block.getIndex( x+1, y,   z   ))];
					Color8 c_top =    m_colorPalette[block.getValueRaw(ATTRIB_COLOR, block.getIndex( x,   y+1, z   ))];
					Color8 c_bottom = m_colorPalette[block.getValueRaw(ATTRIB_COLOR, block.getIndex( x,   y-1, z   ))];
					Color8 c_fore =   m_colorPalette[block.getValueRaw(ATTRIB_COLOR, block.getIndex( x,   y,   z+1 ))];
					Color8 c_back =   m_colorPalette[block.getValueRaw(ATTRIB_COLOR, block.getIndex( x,   y,   z-1 ))];

					if (!isOpaque(c_left))
					{
						makeFace(x, y, z, c, DIR_XN);
					}
					if (!isOpaque(c_right))
					{
						makeFace(x, y, z, c, DIR_XP);
					}
					if (!isOpaque(c_top))
					{
						makeFace(x, y, z, c, DIR_YP);
					}
					if (!isOpaque(c_bottom))
					{
						makeFace(x, y, z, c, DIR_YN);
					}
					if (!isOpaque(c_fore))
					{
						makeFace(x, y, z, c, DIR_ZP);
					}
					if (!isOpaque(c_back))
					{
						makeFace(x, y, z, c, DIR_ZN);
					}
				}
			}
		}
	}

	m_mesh->recalculateIndexes();
	m_mesh->recalculateBounds();

	out_mesh = *m_mesh;

	SN_END_PROFILE_SAMPLE();
}

void MeshBuilder::makeFace(u8 x, u8 y, u8 z, Color8 c, Direction dir)
{
	Mesh & mesh = *m_mesh;
	Color fc = c;

	switch (dir)
	{
	case DIR_XN:
		mesh.addPosition(x, y, z);
		mesh.addPosition(x, y, z+1);
		mesh.addPosition(x, y+1, z+1);
		mesh.addPosition(x, y+1, z);
		for (u32 i = 0; i < 4; ++i)
		{
			mesh.addNormal(-1, 0, 0);
			mesh.addColor(fc);
		}
		break;

		// TODO

	default:
		break;
	}
}

} // namespace voxy

