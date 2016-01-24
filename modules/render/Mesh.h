/*
Mesh.h
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_MESH__
#define __HEADER_SN_MESH__

#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Color.h>
#include <core/math/Area.h>
#include <core/asset/Asset.h>
#include <vector>
#include <array>

#include <modules/render/VertexDescription.h>
#include <modules/render/common.h>
#include <modules/render/MeshPrimitiveType.h>

namespace sn
{

/// \brief Container for render-ready vertex arrays with fully customizable attributes.
class SN_RENDER_API Mesh : public Asset
{
public:
    SN_OBJECT

    struct VertexArray
    {
        std::vector<char> data;
        const VertexAttribute * attribute;
    };

    /// \brief Constructs a mesh with a default vertex format.
    Mesh();

    /// \brief Clears vertices and indices. Doesn't resets the vertex format.
    void clear();

    /// \brief Tells if the mesh has vertices.
    bool isEmpty() const;

    /// \brief Gets the primitive type represented by the indices.
    inline MeshPrimitiveType getPrimitiveType() const { return m_primitiveType; }

    /// \brief Sets the primitive type.
    void setPrimitiveType(MeshPrimitiveType pt);

    /// \brief Gets the non-emulated primitive type
    MeshPrimitiveType getInternalPrimitiveType() const;

    /// \brief Gets the number of non-emulated primitives inside the mesh
    u32 getInternalIndexedPrimitiveCount() const;

    /// \brief Initializes the format of the mesh.
    void create(const VertexDescription & description);

    /// \brief Gets the vertex format
    const VertexDescription & getVertexDescription() const { return m_vertexDescription; }

    /// \brief Sets the values of an attribute for a range of vertices.
    /// If the mesh is too small to contain the data, new vertices will be allocated.
    ///
    /// \param T:             native type of the attribute (float, vec3, color...)
    /// \param use:           attribute index
    /// \param srcData:       array of values
    /// \param dstOffset:     position at which the destination range starts
    ///
    template <typename T>
    void updateArray(u32 use, const std::vector<T> & srcData, u32 dstOffset = 0)
    {
        updateArray(use, srcData.data(), srcData.size(), dstOffset);
    }

    /// \brief Sets the values of an attribute for a range of vertices.
    /// If the mesh is too small to contain the data, new vertices will be allocated.
    ///
    /// \param T:             native type of the attribute (float, vec3, color...)
    /// \param use:           attribute index
    /// \param srcData:       array of values
    /// \param dstOffset:     position at which the destination range starts
    ///
    template <typename T, size_t N>
    void updateArray(u32 use, const std::array<T,N> & srcData, u32 dstOffset = 0)
    {
        updateArray(use, srcData.data(), srcData.size(), dstOffset);
    }

    /// \brief Sets the values of indices describing vertex primitives.
    /// If the mesh is too small to contain the data, new indices will be allocated.
    ///
    /// \param indices:       array of indices
    /// \param count:         number of elements inside indices
    /// \param offset:        position at which the destination range starts
    /// 
    void updateIndices(const u32 * indices, u32 count, u32 offset = 0);

    /// \brief Sets the values of indices describing vertex primitives.
    /// If the mesh is too small to contain the data, new indices will be allocated.
    ///
    /// \param indices:       array of indices
    /// \param count:         number of elements inside indices
    /// \param offset:        position at which the destination range starts
    /// 
    void updateIndices(const std::vector<u32> & indices, u32 offset = 0);

    /// \brief Gets how many vertices are in the mesh (based on positions)
    u32 getVertexCount() const;

    /// \brief Gets the raw data of a vertex array of the mesh.
    /// Currently, it represents the values of one attribute.
    const VertexArray & getVertexArray(u32 use) const { return m_vertexArrays[use]; }

    /// \brief Gets the array of indices
    const std::vector<u32> & getIndices() const { return m_indices; }

    /// \brief Gets bounds of the mesh. No computation is executed.
    const FloatAABB & getBounds() { return m_bounds; }

    /// \brief Manually sets the bounds of the mesh
    void setBounds(const FloatAABB & aabb);

    /// \brief Automatically recalculates the bounds of the mesh from all position attributes.
    /// \warning This method might not work on uncommon vertex formats, If you already know the bounds, please use setBounds().
    void recalculateBounds();

    /// \brief Recalculates the indices of the mesh as if vertices where successively defining the primitives.
    void recalculateIndices();

private:
    /// \brief Sets the values of an attribute for a range of vertices.
    /// If the mesh is too small to contain the data, new vertices will be allocated.
    ///
    /// \param T:             native type of the attribute (float, vec3, color...)
    /// \param use:           attribute index
    /// \param srcData:       array of values
    /// \param srcDataCount:  number of elements inside srcData
    /// \param dstOffset:     position at which the destination range starts
    ///
    template <typename T>
    void updateArray(u32 use, const T * srcData, u32 srcDataCount, u32 dstOffset = 0)
    {
        //const VertexAttributeList & attributes = m_vertexDescription.getAttributes();
        SN_ASSERT(use < m_vertexArrays.size(), "Invalid vertex array index");
        VertexArray & vertexArray = m_vertexArrays[use];
        const u32 srcDataSize = sizeof(T) * srcDataCount;

#ifdef SN_BUILD_DEBUG
        const VertexAttribute * attrib = vertexArray.attribute;
        SN_ASSERT(attrib != nullptr, "Invalid state");
        SN_ASSERT(srcDataSize % attrib->sizeBytes() == 0 , "Invalid data length");
#endif

        if (dstOffset == -1)
            dstOffset = vertexArray.data.size();
        else
            dstOffset = dstOffset * sizeof(T);
        if (vertexArray.data.size() < dstOffset + srcDataSize)
            vertexArray.data.resize(dstOffset + srcDataSize);
        memcpy(vertexArray.data.data() + dstOffset, srcData, srcDataSize);
    }

private:
	VertexDescription m_vertexDescription;
    MeshPrimitiveType m_primitiveType;

	std::vector<VertexArray> m_vertexArrays;

	std::vector<u32> m_indices;

    FloatAABB m_bounds;

};

} // namespace sn

#endif // __HEADER_SN_MESH__

