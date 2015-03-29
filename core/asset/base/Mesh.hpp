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
#include <core/math/Area.hpp>
#include <core/asset/Asset.hpp>
#include <vector>

namespace sn
{

enum MeshPrimitiveType
{
    // Basic primitives
    SN_MESH_POINTS = 0,
    SN_MESH_LINES = 1,
    SN_MESH_TRIANGLES = 2,

    // Emulated primitives
    SN_MESH_QUADS = 3
};

class SN_API Mesh : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::Mesh, sn::Asset)

    Mesh() :
        m_primitiveType(SN_MESH_TRIANGLES)
    {}

    bool canLoad(const AssetMetadata & metadata) const;
    bool loadFromStream(std::ifstream & ifs);

    void clear();
    bool isEmpty() const;

    inline MeshPrimitiveType getPrimitiveType() const { return m_primitiveType; }
    void setPrimitiveType(MeshPrimitiveType pt);

    MeshPrimitiveType getInternalPrimitiveType() const;
    u32 getInternalIndexedPrimitiveCount() const;

    void addPosition(f32 x, f32 y, f32 z=0);
    void addTexCoord(f32 x, f32 y);
    void addNormal(f32 x, f32 y, f32 z);
    void addColor(const Color & c);

    void setPositions(const Vector3f * positions, u32 count);
    void setNormals(const Vector3f * normals, u32 count);
    void setColors(const Color * colors, u32 count);
    void setUV(const Vector2f * uv, u32 count);
    void setCustomFloats(u32 i, const f32 * fdata, u32 count);

    void setQuadIndices(const u32 * indices, u32 count);
    void setTriangleIndices(const u32 * indices, u32 count);

    void recalculateIndexes();
    void recalculateBounds();

    FloatAABB getBounds();

    //static u32 calculateIndicesCount(MeshPrimitiveType primitive, u32 vertexCount);

    inline const std::vector<Vector3f> & getVertices() const { return m_vertices; }
    inline const std::vector<Vector2f> & getUV() const { return m_uv; }
    inline const std::vector<Vector3f> & getNormals() const { return m_normals; }
    inline const std::vector<Color> & getColors() const { return m_colors; }
    inline const std::vector<u32> & getIndices() const { return m_indices; }

    inline const std::vector< std::vector<f32> > & getCustomFloats() const { return m_customFloats; }

//private:
//
//    void host();
//    void unhost();

private:

    MeshPrimitiveType m_primitiveType;

    // TODO Allow more types

    // TODO Make mesh structure completely generic, without hardcode the types one by one?
    // Data would be described in a small structure, and there would be only two buffers:
    // one for vertices, where each one is a memory block of specified size,
    // and one for indices, as unsigned ints.

    std::vector<Vector3f> m_vertices;
    std::vector<Vector2f> m_uv;
    std::vector<Vector3f> m_normals;
    std::vector<Color> m_colors;

    std::vector< std::vector<f32> > m_customFloats;

    std::vector<u32> m_indices;

    FloatAABB m_bounds;

};

std::string toString(MeshPrimitiveType pt);

} // namespace sn

#endif // __HEADER_SN_MESH__

