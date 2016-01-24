#include <core/util/stringutils.h>
#include "Mesh.h"

namespace sn
{

SN_OBJECT_IMPL(Mesh)

//------------------------------------------------------------------------------
Mesh::Mesh() : Asset(), 
    m_primitiveType(SN_MESH_TRIANGLES)
{
    // Default format
    VertexDescription desc;
    desc.addAttribute("Position", VertexAttribute::USE_POSITION, VertexAttribute::TYPE_FLOAT32, 3);
    desc.addAttribute("Color", VertexAttribute::USE_COLOR, VertexAttribute::TYPE_FLOAT32, 4);
    create(desc);
}

//------------------------------------------------------------------------------
void Mesh::clear()
{
    for (auto it = m_vertexArrays.begin(); it != m_vertexArrays.end(); ++it)
    {
        (*it).data.clear();
    }
    m_indices.clear();
}

//------------------------------------------------------------------------------
bool Mesh::isEmpty() const
{
    for (u32 i = 0; i < m_vertexArrays.size(); ++i)
    {
        if (!m_vertexArrays[i].data.empty())
            return false;
    }
    return true;
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
void Mesh::create(const VertexDescription & description)
{
    m_vertexArrays.clear();
    m_vertexDescription = description;
    const VertexAttributeList & attributes = m_vertexDescription.getAttributes();
    for (u32 i = 0; i < attributes.size(); ++i)
    {
        const VertexAttribute & attrib = attributes[i];
        u32 use = attrib.use;
        if (use >= m_vertexArrays.size())
            m_vertexArrays.resize(use + 1);
        m_vertexArrays[use].attribute = &attrib;
    }
}

//------------------------------------------------------------------------------
void Mesh::updateIndices(const u32 * indices, u32 count, u32 offset)
{
    if (offset == -1)
        offset = m_indices.size();
    if (m_indices.size() < offset+count)
        m_indices.resize(offset+count);
    memcpy(m_indices.data() + offset * sizeof(u32), indices, count * sizeof(u32));
}

//------------------------------------------------------------------------------
void Mesh::updateIndices(const std::vector<u32> & indices, u32 offset)
{
    updateIndices(&indices[0], indices.size(), offset);
}

//------------------------------------------------------------------------------
u32 Mesh::getVertexCount() const
{
    if (VertexAttribute::USE_POSITION < m_vertexArrays.size())
    {
        const VertexArray & va = m_vertexArrays[VertexAttribute::USE_POSITION];
#ifdef SN_BUILD_DEBUG
        SN_ASSERT(va.attribute != nullptr, "Invalid state");
#endif
        return va.data.size() / va.attribute->sizeBytes();
    }
    return 0;
}

//------------------------------------------------------------------------------
void Mesh::setBounds(const FloatAABB & aabb)
{
    m_bounds = aabb;
}

//------------------------------------------------------------------------------
void Mesh::recalculateBounds()
{
    FloatAABB aabb;

    bool updated = false;

    if (VertexAttribute::USE_POSITION < m_vertexArrays.size())
    {
        const VertexArray & vertexArray = m_vertexArrays[VertexAttribute::USE_POSITION];
#ifdef SN_BUILD_DEBUG
        SN_ASSERT(vertexArray.attribute != nullptr, "Invalid state");
#endif
        const VertexAttribute & a = *vertexArray.attribute;
        auto & data = vertexArray.data;

        if (a.type == VertexAttribute::TYPE_FLOAT32)
        {
            // TODO Clear code repetition
            if (a.count == 2)
            {
                u32 count = data.size() / sizeof(Vector3f);
                const Vector2f * positions = reinterpret_cast<const Vector2f*>(data.data());

                for (u32 i = 0; i < count; ++i)
                {
                    Vector2f p = positions[i];
                    aabb.addPoint(p.x(), p.y());
                }

                updated = true;
            }
            else if (a.count == 3)
            {
                u32 count = data.size() / sizeof(Vector3f);
                const Vector3f * positions = reinterpret_cast<const Vector3f*>(data.data());

                for (u32 i = 0; i < count; ++i)
                {
                    aabb.addPoint(positions[i]);
                }

                updated = true;
            }
        }
    }
    
#ifdef SN_BUILD_DEBUG
    if (!updated)
        SN_WARNING("Couldn't update mesh bounds: position array not found");
#endif

    m_bounds = aabb;
}

//------------------------------------------------------------------------------
void Mesh::recalculateIndices()
{
    switch (m_primitiveType)
    {
    case SN_MESH_POINTS:
    case SN_MESH_LINES:
    case SN_MESH_TRIANGLES:
        m_indices.resize(getVertexCount());
        for (u32 i = 0; i < m_indices.size(); ++i)
        {
            m_indices[i] = i;
        }
        break;

    case SN_MESH_QUADS:
    {
        // Make triangles out of quad data
        u32 quadCount = getVertexCount() / 4;
        u32 indiceCount = quadCount * 6;
        m_indices.resize(indiceCount);
        for (u32 j = 0; j < quadCount; ++j)
        {
            u32 i = j * 6;
            u32 ti = j * 4;
            m_indices[i] = ti;
            m_indices[i + 1] = ti + 2;
            m_indices[i + 2] = ti + 1;
            m_indices[i + 3] = ti;
            m_indices[i + 4] = ti + 3;
            m_indices[i + 5] = ti + 2;
        }
        break;
    }

    default:
        SN_ERROR("Primitive type " << m_primitiveType << " not supported yet");
        break;
    }
}

} // namespace sn
