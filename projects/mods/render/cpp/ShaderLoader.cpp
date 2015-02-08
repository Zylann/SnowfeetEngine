#include <core/util/stringutils.hpp>
#include <core/util/Log.hpp>

#include "ShaderLoader.hpp"
#include "ShaderProgram.hpp"

namespace sn {
namespace render {

bool ShaderLoader::loadFromFile(ShaderProgram & shaderProgram, const std::string & filePath)
{
    std::string ext = getFileExtension(filePath);

    if (ext == ".shader")
    {
        // Load the shader from a merged file
        return loadMergedShaderFromFile(shaderProgram, filePath);
    }
    else
    {
        SN_ERROR("Cannot load shader file, the \"" << ext << "\" file extension is not supported.");
        return false;
    }

    return true;
}

bool ShaderLoader::loadMergedShaderFromFile(ShaderProgram & shaderProgram, const std::string & filePath)
{
    std::ifstream ifs(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!ifs.good())
    {
        SN_ERROR("Couldn't open merged shader file \"" << filePath << '"');
        return false;
    }

    const unsigned int VERTEX = SNR_ST_VERTEX;
    const unsigned int FRAGMENT = SNR_ST_FRAGMENT;
    const unsigned int NO_MODE = 2;

    std::string shaderStr[2];

    unsigned int mode = NO_MODE;

    const std::string vertexCommand = "#vertex";
    const std::string fragmentCommand = "#fragment";

    // Extract each shader types from the file
    while (!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        line += '\n';

        if (!line.empty() && line[0] == '#')
        {
            if (line.find(vertexCommand) != std::string::npos)
            {
                // Treat next lines as a vertex program
                mode = VERTEX;
                shaderStr[0] += '\n';
                shaderStr[1] += '\n';
                continue;
            }
            else if (line.find(fragmentCommand) != std::string::npos)
            {
                // Treat next lines as a fragment program
                mode = FRAGMENT;
                shaderStr[0] += '\n';
                shaderStr[1] += '\n';
                continue;
            }
        }

        // Append code to the right shader, and append
        // newlines in the other to keep line numbers in sync with the
        // merged file
        switch (mode)
        {
        case VERTEX:
            shaderStr[VERTEX] += line;
            shaderStr[FRAGMENT] += '\n';
            break;

        case FRAGMENT:
            shaderStr[VERTEX] += '\n';
            shaderStr[FRAGMENT] += line;
            break;

        default:
            shaderStr[VERTEX] += '\n';
            shaderStr[FRAGMENT] += '\n';
            break;
        }
    }

    ifs.close();

    // Load the shader
    // TODO Optimize this
    std::unordered_map<ShaderType, std::string> sources;
    sources[SNR_ST_VERTEX] = shaderStr[VERTEX];
    sources[SNR_ST_FRAGMENT] = shaderStr[VERTEX];
    if (!shaderProgram.loadFromSourceCode(sources))
    {
        SN_ERROR("Reading merged shader file \"" << filePath << '"');
        return false;
    }

    return true;
}

} // namespace render
} // namespace sn


