#include <core/util/Log.hpp>
#include "ShaderLoader.hpp"
#include "ShaderProgram.hpp"

namespace sn {
namespace render {

//==============================================================================
// Enums
//==============================================================================

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

std::string toString(VertexAttribute attrib)
{
    switch (attrib)
    {
    case SNR_ATTRIB_POSITION: return "Position";
    case SNR_ATTRIB_COLOR: return "Color";
    case SNR_ATTRIB_TEXCOORD: return "Texcoord";
    case SNR_ATTRIB_NORMAL: return "Normal";
    default: return "VertexAttrib[" + std::to_string((s32)attrib) + "]";
    }
}

//==============================================================================
// ShaderProgram
//==============================================================================

ShaderProgram::~ShaderProgram()
{
    unload();
}

void ShaderProgram::unload()
{
    if (m_programID)
    {
        for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
        {
            Shader * shader = *it;
            if (shader)
            {
                glDetachShader(m_programID, shader->ID);
                glDeleteShader(shader->ID);
                delete shader;
            }
        }

        glDeleteProgram(m_programID);
        m_programID = 0;
    }

    m_shaders.clear();
}

bool ShaderProgram::loadFromFile(const std::string & filePath)
{
    return ShaderLoader::loadFromFile(*this, filePath);
}

bool ShaderProgram::loadFromSourceCode(const std::unordered_map<ShaderType, std::string> & sources)
{
    // Check if none of the sources is empty
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
        if (m_shaders.size() < shaderType)
            m_shaders.resize(shaderType + 1, nullptr);
        m_shaders[shaderType] = new Shader(sID);
    }

    // Link shaders into a program
    SN_LOG("linking shader program...");
    m_programID = glCreateProgram();
    // Attach shaders to the program
    for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
        Shader * shader = *it;
        if (shader)
        {
            glAttachShader(m_programID, shader->ID);
        }
    }

    // Generic input shader variables (only for OpenGL3-based experimental version)
    //		glBindAttribLocation(m_programID, Attrib::POSITION, "in_Position");
    //		glBindAttribLocation(m_programID, Attrib::COLOR, "in_Color");
    //		glBindAttribLocation(m_programID, Attrib::TEXCOORD0, "in_TexCoord0");
    //		glBindAttribLocation(m_programID, Attrib::NORMAL, "in_Normal");

    // Link
    glLinkProgram(m_programID);

    // Check errors
    GLint isLinked = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &isLinked);
    if (isLinked != GL_TRUE)
    {
        // Retrieve error log size
        GLint errorSize = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &errorSize);

        // This string will contain the error message
        char * errorStr = new char[errorSize + 1];
        errorStr[errorSize] = '\0';

        // Retrieve the error log
        glGetProgramInfoLog(m_programID, errorSize, &errorSize, errorStr);

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

// Static
bool ShaderProgram::loadShaderFromSourceCode(GLuint & outShaderID, ShaderType typeGeneric, const std::string & source)
{
    SN_LOG("Compiling shader type " << toString(typeGeneric) << "... ");

    // Create and compile shader
    GLenum type = shaderTypeToGL(typeGeneric);
    outShaderID = glCreateShader(type);
    if (outShaderID == 0)
    {
        SN_ERROR("ShaderProgram::loadShaderFromSourceCode: Unable to create shader. Cause: unknown type (" << toString(typeGeneric) << ")");
        return false;
    }

    const GLchar * sourceStr = source.c_str();

    glShaderSource(outShaderID, 1, &sourceStr, 0);
    glCompileShader(outShaderID);

    // Check compilation
    GLint isCompiled = 0;
    glGetShaderiv(outShaderID, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled != GL_TRUE)
    {
        // Retrieve error log size
        GLint errorSize = 0;
        glGetShaderiv(outShaderID, GL_INFO_LOG_LENGTH, &errorSize);

        // This string will contain the error message
        char * errorStr = new char[errorSize + 1];
        errorStr[errorSize] = '\0';

        // Retrieve the error log
        glGetShaderInfoLog(outShaderID, errorSize, &errorSize, errorStr);

        // Display the error
        SN_ERROR("ShaderProgram::loadShader: Compile error(s). (shader type is " << toString(typeGeneric) << ")");
        SN_ERROR(errorStr);

        // Free memory and return
        delete[] errorStr;
        glDeleteShader(outShaderID);

        return false; // Error
    }

    return true; // Fine !
}

void ShaderProgram::setParam(const std::string & name, f32 value)
{
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setParam(const std::string & name, f32 x, f32 y)
{
    glUniform2f(getUniformLocation(name), x, y);
}

void ShaderProgram::setParam(const std::string & name, f32 x, f32 y, f32 z)
{
    glUniform3f(getUniformLocation(name), x, y, z);
}

void ShaderProgram::setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w)
{
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setParam(const std::string & name, const f32 matrixValues[16], bool transpose)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, transpose, matrixValues);
}

GLint ShaderProgram::getUniformLocation(const std::string & name)
{
    GLint loc;
    auto it = m_uniforms.find(name);
    if (it == m_uniforms.end())
    {
        loc = glGetUniformLocation(m_programID, name.c_str());
        m_uniforms[name] = loc;
    }
    else
    {
        loc = it->second;
    }
    return loc;
}

} // namespace render
} // namespace sn


