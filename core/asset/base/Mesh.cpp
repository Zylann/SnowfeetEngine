#include <core/util/stringutils.hpp>
#include "Mesh.hpp"

namespace sn {

//------------------------------------------------------------------------------
std::string toString(MeshPrimitiveType pt)
{
    switch (pt)
    {
    case SN_MESH_POINTS: return "Points"; break;
    case SN_MESH_LINES: return "Lines"; break;
    case SN_MESH_TRIANGLES: return "Lines"; break;
    case SN_MESH_QUADS: return "Quads"; break;
    default: return "MeshPrimitiveType[" + std::to_string((s32)pt) + "]"; break;
    }
}

//------------------------------------------------------------------------------
bool Mesh::canLoad(const AssetMetadata & metadata) const
{
    return false;
}

//------------------------------------------------------------------------------
bool Mesh::loadFromStream(std::ifstream & ifs)
{
    return false;
}

//------------------------------------------------------------------------------
void Mesh::clear()
{
    m_vertices.clear();
    m_uv.clear();
    m_normals.clear();
    m_colors.clear();
    m_indices.clear();
}

//------------------------------------------------------------------------------
bool Mesh::isEmpty() const
{
    return m_vertices.empty();
}

//------------------------------------------------------------------------------
void Mesh::setPrimitiveType(MeshPrimitiveType pt)
{
    m_primitiveType = pt;
}

//------------------------------------------------------------------------------
MeshPrimitiveType Mesh::getInternalPrimitiveType() const
{
    switch (m_primitiveType)
    {
    case SN_MESH_QUADS:
        return SN_MESH_TRIANGLES;
    default:
        return m_primitiveType;
    }
}

//------------------------------------------------------------------------------
u32 Mesh::getInternalIndexedPrimitiveCount() const
{
    switch (m_primitiveType)
    {
    case SN_MESH_POINTS:
        return m_indices.size();

    case SN_MESH_LINES:
        return m_indices.size() / 2;

    case SN_MESH_QUADS: // Emulated with triangles
    case SN_MESH_TRIANGLES:
        return m_indices.size() / 3;

    default:
        SN_ASSERT(false, "Invalid state");
        return 0;
    }
}

//------------------------------------------------------------------------------
void Mesh::addPosition(f32 x, f32 y, f32 z)
{
    m_vertices.push_back(Vector3f(x, y, z));
}

//------------------------------------------------------------------------------
void Mesh::addTexCoord(f32 x, f32 y)
{
    m_uv.push_back(Vector2f(x, y));
}

//------------------------------------------------------------------------------
void Mesh::addNormal(f32 x, f32 y, f32 z)
{
    m_normals.push_back(Vector3f(x, y, z));
}

//------------------------------------------------------------------------------
void Mesh::addColor(const Color & c)
{
    m_colors.push_back(c);
}

//------------------------------------------------------------------------------
//void Mesh::addIndex(u32 i)
//{
//    m_indices.push_back(i);
//}

//------------------------------------------------------------------------------
// Static
//u32 Mesh::calculateIndicesCount(MeshPrimitiveType primitive, u32 primitiveCount)
//{
//    switch (primitive)
//    {
//    case SNR_PT_LINES:
//        return primitiveCount * 2;
//    //case SNR_PT_LINESTRIP:
//    //    return primitiveCount + 1;
//    case SNR_PT_TRIANGLES:
//        return primitiveCount * 3;
//    case SNR_PT_QUADS:
//        return primitiveCount * 4;
//    }
//}

//------------------------------------------------------------------------------
void Mesh::recalculateIndexes()
{
    switch (m_primitiveType)
    {
    case SN_MESH_POINTS:
    case SN_MESH_LINES:
    case SN_MESH_TRIANGLES:
        m_indices.resize(m_vertices.size());
        for (u32 i = 0; i < m_indices.size(); ++i)
        {
            m_indices[i] = i;
        }
        break;

    case SN_MESH_QUADS:
    {
        // Make triangles out of quad data
        u32 quadCount = m_vertices.size() / 4;
        u32 indiceCount = quadCount * 6;
        m_indices.resize(indiceCount);
        for (u32 j = 0; j < quadCount; ++j)
        {
            u32 i = j * 4;
            m_indices[i] = i;
            m_indices[i + 1] = i + 2;
            m_indices[i + 2] = i + 1;
            m_indices[i + 3] = i;
            m_indices[i + 4] = i + 3;
            m_indices[i + 5] = i + 2;
        }
        break;
    }

    default:
        SN_ERROR("Primitive type " << m_primitiveType << " not supported yet");
        break;
    }
}

//------------------------------------------------------------------------------
void Mesh::setPositions(const Vector3f * positions, u32 count)
{
    SN_ASSERT(positions != nullptr, "Invalid positions pointer");
    m_vertices.resize(count);
    if (count)
        memcpy(&m_vertices[0], positions, sizeof(Vector3f)*count);
}

//------------------------------------------------------------------------------
void Mesh::setNormals(const Vector3f * normals, u32 count)
{
    SN_ASSERT(normals != nullptr, "Invalid normals pointer");
    m_normals.resize(count);
    if (count)
        memcpy(&m_normals[0], normals, sizeof(Vector3f)*count);
}

//------------------------------------------------------------------------------
void Mesh::setColors(const Color * colors, u32 count)
{
    SN_ASSERT(colors != nullptr, "Invalid colors pointer");
    m_colors.resize(count);
    if (count)
        memcpy(&m_colors[0], colors, sizeof(Color)*count);
}

//------------------------------------------------------------------------------
void Mesh::setUV(const Vector2f * uv, u32 count)
{
    SN_ASSERT(uv != nullptr, "Invalid colors pointer");
    m_uv.resize(count);
    if (count)
        memcpy(&m_uv[0], uv, sizeof(Vector2f)*count);
}

//------------------------------------------------------------------------------
void Mesh::setCustomFloats(u32 i, const f32 * fdata, u32 count)
{
    SN_ASSERT(fdata != nullptr, "Invalid custom floats pointer");
    if (i >= m_customFloats.size())
        m_customFloats.resize(i + 1);
    auto & buffer = m_customFloats[i];
    buffer.resize(count);
    if (count)
        memcpy(&buffer[0], fdata, sizeof(f32)*count);
}

//------------------------------------------------------------------------------
void Mesh::setQuadIndices(const u32 * indices, u32 count)
{
    SN_ASSERT(indices != nullptr, "Invalid indices pointer");
    SN_ASSERT(count % 4 == 0, "Number of quad indices array is not a multiple of 4");
    if (count)
    {
        m_indices.resize(6 * (count / 4));
        u32 j = 0;
        for (u32 i = 0; i < count; i += 4)
        {
            m_indices[j] = indices[i + 2];
            m_indices[j + 1] = indices[i + 3];
            m_indices[j + 2] = indices[i + 1];
            m_indices[j + 3] = indices[i + 3];
            m_indices[j + 4] = indices[i];
            m_indices[j + 5] = indices[i + 1];
            j += 6;
        }
    }
}

//------------------------------------------------------------------------------
void Mesh::setTriangleIndices(const u32 * indices, u32 count)
{
    SN_ASSERT(indices != nullptr, "Invalid indices pointer");
    SN_ASSERT(count % 3 == 0, "Number of triangle indices array is not a multiple of 3");
    m_indices.resize(count);
    if (count)
        memcpy(&m_indices[0], indices, count * sizeof(u32));
}

} // namespace sn

