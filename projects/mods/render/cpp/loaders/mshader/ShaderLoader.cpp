#include <core/util/stringutils.hpp>
#include <core/util/Log.hpp>

#include "ShaderLoader.hpp"
#include "../../ShaderProgram.hpp"

namespace sn {
namespace render {


//------------------------------------------------------------------------------
std::string ShaderLoader::extractPreprocessorCommand(const std::string & str)
{
    u32 i = 0;
    for (; i < str.size(); ++i)
    {
        char c = str[i];
        if (c == ' ' || c == '\t')
            continue;
        if (c == '#')
            break;
        else
            return "";
    }
    if (i < str.size())
        return trim(str.substr(i + 1));
    else
        return "";
}

//------------------------------------------------------------------------------
bool ShaderLoader::loadMergedShaderFromStream(ShaderProgram & shaderProgram, std::ifstream & ifs)
{
    if (!ifs.good())
    {
        SN_ERROR("Couldn't read merged shader stream");
        return false;
    }

    std::string sources[SNR_ST_COUNT];
    bool openSection[SNR_ST_COUNT] = { false };
    
    std::map<std::string, ShaderType> commandToShaderType;
    commandToShaderType["msh_vertex"] = SNR_ST_VERTEX;
    commandToShaderType["msh_fragment"] = SNR_ST_FRAGMENT;
    commandToShaderType["msh_geometry"] = SNR_ST_GEOMETRY;

    ShaderType currentShaderType = SNR_ST_COUNT;
    u32 lineNumber = 1;

    // Extract each shader types from the file
    while (!ifs.eof())
    {
        // Get current line
        std::string line;
        std::getline(ifs, line);
        line += '\n';

        // Find command in it
        std::string command = extractPreprocessorCommand(line);
        auto commandIt = command.empty() ? commandToShaderType.end() : commandToShaderType.find(command);

        // If a command was found
        if (commandIt != commandToShaderType.end())
        {
            currentShaderType = commandIt->second;

            // Check for duplicate sections
            if (openSection[currentShaderType])
            {
                SN_ERROR("Compiling merged shader: section #" << command << " declared twice (line " << lineNumber << ")");
                return false;
            }

            openSection[currentShaderType] = true;

            // Append newline in each sources to replace the command
            for (u32 i = 0; i < SNR_ST_COUNT; ++i)
                sources[i] += '\n';

            continue;
        }

        // Append newline in each sources, except for the current section, in which the actual line is added.
        // (newlines ensure that line numbers will be consistent when the shader will be compiled)
        for (u32 i = 0; i < SNR_ST_COUNT; ++i)
        {
            if (i == currentShaderType)
                sources[i] += line;
            else
                sources[i] += '\n';
        }

        ++lineNumber;
    }

    // Load the shader
    std::unordered_map<ShaderType, std::string> sourcesMap;
    for (u32 i = 0; i < SNR_ST_COUNT; ++i)
    {
        if (openSection[i])
            sourcesMap[(ShaderType)i] = sources[i];
    }
    if (!shaderProgram.loadFromSourceCode(sourcesMap))
    {
        SN_ERROR("Compiling merged shader");
        return false;
    }

    return true;
}

} // namespace render
} // namespace sn


