#ifndef __HEADER_SN_RENDER_SHADERLOADER__
#define __HEADER_SN_RENDER_SHADERLOADER__

#include "ShaderLoader.hpp"

namespace sn {
namespace render {

class ShaderProgram;

class ShaderLoader
{
public:
    static bool loadFromFile(ShaderProgram & shaderProgram, const std::string & filePath);
    static bool loadMergedShaderFromFile(ShaderProgram & shaderProgram, const std::string & filePath);
};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_SHADERLOADER__


