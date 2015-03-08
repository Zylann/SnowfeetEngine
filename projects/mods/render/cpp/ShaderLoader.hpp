#ifndef __HEADER_SN_RENDER_SHADERLOADER__
#define __HEADER_SN_RENDER_SHADERLOADER__

namespace sn {
namespace render {

class ShaderProgram;

class ShaderLoader
{
public:
    static bool loadMergedShaderFromStream(ShaderProgram & shaderProgram, std::ifstream & ifs);

private:
    static std::string extractPreprocessorCommand(const std::string & str);

    ShaderLoader() {}
};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_SHADERLOADER__


