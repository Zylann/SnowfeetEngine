/*
Mesh.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MESH__
#define __HEADER_SN_MESH__

#include <core/math/Vector2.hpp>
#include <core/math/Vector3.hpp>
#include <core/math/Color.hpp>
#include <vector>

namespace sn
{

enum PrimitiveType
{
    SNR_PT_POINTS,
    SNR_PT_LINES,
    SNR_PT_LINESTRIP,
    SNR_PT_TRIANGLES,
    SNR_PT_TRIANGLESTRIP,
    SNR_PT_QUADS
};

class Mesh
{
public:

    Mesh() :
        m_primitiveType(SNR_PT_TRIANGLES)
    {}

    // TODO Mesh

private:

    PrimitiveType m_primitiveType;

    std::vector<Vector3f> m_vertices;
    std::vector<Vector2f> m_uv;
    std::vector<Vector3f> m_normals;
    std::vector<Color> m_colors;
    std::vector<int> m_indices;

};

} // namespace sn

#endif // __HEADER_SN_MESH__

