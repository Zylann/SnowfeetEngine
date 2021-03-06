﻿#include <core/util/Log.h>

#include "VertexAttribute.h"
#include "gl_check.h"
#include "ShaderProgram.h"
#include <GL/glew.h>

namespace sn
{

SN_OBJECT_IMPL(ShaderProgram)

//==============================================================================
// Enums
//==============================================================================

//------------------------------------------------------------------------------
GLuint shaderTypeToGL(ShaderType st)
{
    switch (st)
    {
    case SNR_ST_VERTEX: return GL_VERTEX_SHADER; break;
    case SNR_ST_FRAGMENT: return GL_FRAGMENT_SHADER; break;
    case SNR_ST_GEOMETRY: return GL_GEOMETRY_SHADER; break;
    default:
        SN_ASSERT(false, "Invalid state");
        return 0;
    }
}

//------------------------------------------------------------------------------
std::string toString(ShaderType st)
{
    switch (st)
    {
    case SNR_ST_VERTEX: return "Vertex";
    case SNR_ST_FRAGMENT: return "Fragment";
    case SNR_ST_GEOMETRY: return "Geometry";
    default: return "ShaderType[" + std::to_string((s32)st) + "]";
    }
}

//==============================================================================
// ShaderProgram
//==============================================================================

//------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
    unload();
}

//------------------------------------------------------------------------------
void ShaderProgram::unload()
{
    // TODO FIXME OpenGL fails here because contextes are destroyed before assets to be released...
    // Maybe services such as RenderManager should be destroyed after scene objects? Or handle services differently?
    if (m_programID)
    {
        for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
        {
            Shader * shader = *it;
            if (shader)
            {
                glCheck(glDetachShader(m_programID, shader->ID));
                glCheck(glDeleteShader(shader->ID));
                delete shader;
            }
        }

        glCheck(glDeleteProgram(m_programID));
        m_programID = 0;
    }

    m_shaders.clear();
}

//------------------------------------------------------------------------------
bool ShaderProgram::loadFromSourceCode(const std::unordered_map<ShaderType, std::string> & sources)
{
    // Check if none of the sources is empty
    if (sources.empty())
    {
        SN_ERROR("ShaderProgram::loadFromSourceCode: source code is empty");
        return false;
    }
    for (auto it = sources.begin(); it != sources.end(); ++it)
    {
        if (it->second.empty())
        {
            SN_ERROR("ShaderProgram::loadFromSourceCode: source code is empty for shader type " << toString(it->first));
            return false;
        }
    }

    // Deletes the old program if it was already loaded
    unload();

    // Reset uniforms because they might change if the shader is reloading
    m_uniforms.clear();

    // Create shaders
    GLuint sID;
    for (auto it = sources.begin(); it != sources.end(); ++it)
    {
        ShaderType shaderType = it->first;
        if (!loadShaderFromSourceCode(sID, shaderType, it->second))
        {
            // Error
            unload();
            return false;
        }
        if (static_cast<u32>(shaderType) >= m_shaders.size()) // Note: enums are ints, not uints
            m_shaders.resize(shaderType + 1, nullptr);
        m_shaders[shaderType] = new Shader(sID);
    }

    // Link shaders into a program
    SN_LOG("linking shader program...");
    m_programID = glCreateProgram(); // TODO Verify output value?
    // Attach shaders to the program
    for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
        Shader * shader = *it;
        if (shader)
        {
            glCheck(glAttachShader(m_programID, shader->ID));
        }
    }

    // Generic input shader variables
    glCheck(glBindAttribLocation(m_programID, VertexAttribute::USE_POSITION, "in_Position"));
    glCheck(glBindAttribLocation(m_programID, VertexAttribute::USE_COLOR, "in_Color"));
    glCheck(glBindAttribLocation(m_programID, VertexAttribute::USE_TEXCOORD, "in_TexCoord"));
    glCheck(glBindAttribLocation(m_programID, VertexAttribute::USE_NORMAL, "in_Normal"));

    // Link
    glCheck(glLinkProgram(m_programID));

    // Check errors
    GLint isLinked = 0;
    glCheck(glGetProgramiv(m_programID, GL_LINK_STATUS, &isLinked));
    if (isLinked != GL_TRUE)
    {
        // Retrieve error log size
        GLint errorSize = 0;
        glCheck(glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &errorSize));

        // This string will contain the error message
        char * errorStr = new char[errorSize + 1];
        errorStr[errorSize] = '\0';

        // Retrieve the error log
        glCheck(glGetProgramInfoLog(m_programID, errorSize, &errorSize, errorStr));

        // Display the error
        SN_ERROR("ShaderProgram::load: Link error(s).");
        SN_ERROR(errorStr);

        // Free memory and return
        delete[] errorStr;
        unload(); // Rollback
        return false; // Error
    }

    return true; // Fine !
}

//------------------------------------------------------------------------------
// Static
bool ShaderProgram::loadShaderFromSourceCode(GLuint & outShaderID, ShaderType typeGeneric, const std::string & source)
{
    SN_LOG("Compiling shader type " << toString(typeGeneric) << "... ");

    // Create and compile shader
    GLenum type = shaderTypeToGL(typeGeneric);
    outShaderID = glCreateShader(type); // TODO Verify output?
    if (outShaderID == 0)
    {
        SN_ERROR("ShaderProgram::loadShaderFromSourceCode: Unable to create shader. Cause: unknown type (" << toString(typeGeneric) << ")");
        return false;
    }

    const GLchar * sourceStr = source.c_str();

    glCheck(glShaderSource(outShaderID, 1, &sourceStr, 0));
    glCheck(glCompileShader(outShaderID));

    // Check compilation
    GLint isCompiled = 0;
    glCheck(glGetShaderiv(outShaderID, GL_COMPILE_STATUS, &isCompiled));
    if (isCompiled != GL_TRUE)
    {
        // Retrieve error log size
        GLint errorSize = 0;
        glCheck(glGetShaderiv(outShaderID, GL_INFO_LOG_LENGTH, &errorSize));

        // This string will contain the error message
        char * errorStr = new char[errorSize + 1];
        errorStr[errorSize] = '\0';

        // Retrieve the error log
        glCheck(glGetShaderInfoLog(outShaderID, errorSize, &errorSize, errorStr));

        // Display the error
        SN_ERROR("ShaderProgram::loadShader: Compile error(s). (shader type is " << toString(typeGeneric) << ")");
        SN_ERROR(errorStr);

        // Free memory and return
        delete[] errorStr;
        glCheck(glDeleteShader(outShaderID));

        return false; // Error
    }

    return true; // Fine !
}

// TODO FIXME When the shader has not compiled, params setting should do nothing.
// because the compile error is already shown to the user

//------------------------------------------------------------------------------
void ShaderProgram::setParam(const std::string & name, f32 value)
{
    glCheck(glUniform1f(getUniformLocation(name), value));
}

//------------------------------------------------------------------------------
void ShaderProgram::setParam(const std::string & name, f32 x, f32 y)
{
    glCheck(glUniform2f(getUniformLocation(name), x, y));
}

//------------------------------------------------------------------------------
void ShaderProgram::setParam(const std::string & name, f32 x, f32 y, f32 z)
{
    glCheck(glUniform3f(getUniformLocation(name), x, y, z));
}

//------------------------------------------------------------------------------
void ShaderProgram::setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w)
{
    glCheck(glUniform4f(getUniformLocation(name), x, y, z, w));
}

//------------------------------------------------------------------------------
void ShaderProgram::setParam(const std::string & name, const f32 matrixValues[16], bool transpose)
{
    glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, transpose, matrixValues));
}

//------------------------------------------------------------------------------
void ShaderProgram::setParam(const std::string & name, s32 i)
{
    glCheck(glUniform1i(getUniformLocation(name), i));
}

//------------------------------------------------------------------------------
bool ShaderProgram::hasParameter(const std::string & name)
{
    return getUniformLocation(name) != -1;
}

//------------------------------------------------------------------------------
GLint ShaderProgram::getUniformLocation(const std::string & name)
{
    GLint loc;
    auto it = m_uniforms.find(name);
    if (it == m_uniforms.end())
    {
        loc = glCheck(glGetUniformLocation(m_programID, name.c_str()));
        m_uniforms[name] = loc;
    }
    else
    {
        loc = it->second;
    }
    return loc;
}

} // namespace sn


