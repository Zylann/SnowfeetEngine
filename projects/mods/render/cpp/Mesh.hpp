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
#include <core/app/ScriptObject.hpp>
#include <vector>
#include <GL/glew.h>

namespace sn {
namespace render {

enum PrimitiveType
{
    // Basic primitives
    SNR_PT_POINTS         = 0,
    SNR_PT_LINES          = 1,
    SNR_PT_TRIANGLES      = 2,

    // Emulated primitives
    SNR_PT_QUADS          = 3
};

class Mesh : public ScriptObject
{
public:
    SN_SCRIPT_OBJECT(sn::render::Mesh, sn::ScriptObject)

    Mesh() :
        m_primitiveType(SNR_PT_TRIANGLES),
        m_vaoID(0)
    {}

    void clear();
    bool isEmpty() const;

    inline PrimitiveType getPrimitiveType() const { return m_primitiveType; }
    void setPrimitiveType(PrimitiveType pt);

    GLenum getInternalPrimitiveType() const;

    void addPosition(f32 x, f32 y, f32 z=0);
    void addTexCoord(f32 x, f32 y);
    void addNormal(f32 x, f32 y, f32 z);
    void addColor(const Color & c);

    void recalculateIndexes();

    //static u32 calculateIndicesCount(PrimitiveType primitive, u32 vertexCount);

    inline const std::vector<Vector3f> & getVertices() const { return m_vertices; }
    inline const std::vector<Vector2f> & getUV() const { return m_uv; }
    inline const std::vector<Vector3f> & getNormals() const { return m_normals; }
    inline const std::vector<Color> & getColors() const { return m_colors; }
    inline const std::vector<s32> & getIndices() const { return m_indices; }

//private:
//
//    void host();
//    void unhost();

private:

    PrimitiveType m_primitiveType;

    // TODO Allow more types

    std::vector<Vector3f> m_vertices;
    std::vector<Vector2f> m_uv;
    std::vector<Vector3f> m_normals;
    std::vector<Color> m_colors;
    std::vector<s32> m_indices;

    GLuint m_vaoID;

};

std::string toString(PrimitiveType pt);

} // namespace render
} // namespace sn

#endif // __HEADER_SN_MESH__

