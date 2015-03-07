#include "Mesh.hpp"

namespace sn {
namespace render {

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
void Mesh::setPrimitiveType(PrimitiveType pt)
{
    m_primitiveType = pt;
}

//------------------------------------------------------------------------------
GLenum Mesh::getInternalPrimitiveType() const
{
    switch (m_primitiveType)
    {
    case SNR_PT_POINTS: return GL_POINTS;
    case SNR_PT_LINES: return GL_LINES;

    case SNR_PT_TRIANGLES:
    case SNR_PT_QUADS:
        return GL_TRIANGLES;

    default:
        return GL_LINES;
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
//u32 Mesh::calculateIndicesCount(PrimitiveType primitive, u32 primitiveCount)
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
    case SNR_PT_POINTS:
    case SNR_PT_LINES:
    case SNR_PT_TRIANGLES:
        m_indices.resize(m_vertices.size());
        for (u32 i = 0; i < m_indices.size(); ++i)
        {
            m_indices[i] = i;
        }
        break;

    case SNR_PT_QUADS:
    {
        // Make triangles out of quad data
        u32 count = (m_vertices.size() / 4) * 3;
        m_indices.resize(count);
        for (u32 j = 0; j < count; ++j)
        {
            u32 i = j * 4;
            m_indices[i] = i;
            m_indices[i + 1] = i + 1;
            m_indices[i + 2] = i + 2;
            m_indices[i + 3] = i;
            m_indices[i + 4] = i + 2;
            m_indices[i + 5] = i + 3;
        }
    }

    default:
        SN_ERROR("Primitive type " << m_primitiveType << " not supported yet");
        break;
    }
}

//------------------------------------------------------------------------------
std::string toString(PrimitiveType pt)
{
    switch (pt)
    {
    case SNR_PT_POINTS: return "Points"; break;
    case SNR_PT_LINES: return "Lines"; break;
    case SNR_PT_TRIANGLES: return "Lines"; break;
    case SNR_PT_QUADS: return "Quads"; break;
    default: return "PrimitiveType[" + std::to_string((s32)pt) + "]"; break;
    }
}

} // namespace render
} // namespace sn

