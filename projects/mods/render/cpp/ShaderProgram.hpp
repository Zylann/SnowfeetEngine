/*
Shader.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_RENDER_SHADERPROGRAM__
#define __HEADER_SN_RENDER_SHADERPROGRAM__

#include <core/asset/Asset.hpp>
#include <unordered_map>
#include <GL/glew.h>

namespace sn {
namespace render {

// for use in OpenGL3
enum VertexAttribute
{
    SNR_ATTRIB_POSITION = 0,
    SNR_ATTRIB_COLOR = 1,
    SNR_ATTRIB_TEXCOORD = 2,
    SNR_ATTRIB_NORMAL = 3
};

class SN_API ShaderProgram : public Asset
{
public:

    SN_ASSET("shader");

    // Constructs an empty program.
    ShaderProgram():
        Asset(),
        m_programID(0),
        m_vertex(nullptr),
        m_geometry(nullptr),
        m_fragment(nullptr)
    {}

    bool loadFromFile(const std::string & filePath) override
    {
        // TODO
        return false;
    }

    // Compiles and links the program from up to 3 shader types.
    // An empty source path means no shader, but at least 1 must be specified.
    // The old program will be erased if called twice.
    bool loadFromFiles(
        const std::string & vertSourcePath,
        const std::string & geomSourcePath,
        const std::string & fragSourcePath
    );

    // load() shortcut with only vertex and fragment shader
    inline bool loadFromFiles(
        const std::string & vertSourcePath,
        const std::string & fragSourcePath)
    {
        return loadFromFiles(vertSourcePath, "", fragSourcePath);
    }

    // Deletes the program and its shaders.
    void unload();

    // Returns the program's ID.
    inline GLuint getID() const { return m_programID; }

    void setParam(const std::string & name, f32 x);
    void setParam(const std::string & name, f32 x, f32 y);
    void setParam(const std::string & name, f32 x, f32 y, f32 z);
    void setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w);
    void setParam(const std::string & name, const f32 matrixValues[16], bool transpose=false);

protected:
    // Destroys the program and its shaders.
    ~ShaderProgram();

private:

    // Finds the location of an uniform using internal index.
    // If not found in the index, the program is queried.
    // Undefined uniform name leads to an undefined behavior.
    GLint getUniformLocation(const std::string & name);

    // Loads a shader from a source file,
    // and returns its ID in outShaderID.
    // Returns true if success, false if not.
    static bool loadShader(
        GLuint & outShaderID, GLenum type,
        const std::string & sourcePath
    );

    struct Shader
    {
        std::string sourcePath;
        GLuint ID;

        Shader(GLuint sID, const std::string path):
            sourcePath(path), ID(sID)
        {}
    };

    GLuint m_programID;

    // Note: Uniforms affect a geometry (while attribs affect vertices)
    std::unordered_map<std::string, GLint> m_uniforms;

    Shader * m_vertex;
    Shader * m_geometry;
    Shader * m_fragment;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_SHADERPROGRAM__

