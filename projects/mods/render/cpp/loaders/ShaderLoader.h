#ifndef __HEADER_SNR_SHADERLOADER__
#define __HEADER_SNR_SHADERLOADER__

#include <core/asset/AssetLoader.hpp>
#include "../ShaderProgram.hpp"

namespace sn {
namespace render {

class ShaderLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::render::ShaderLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

private:
    static bool loadMergedShaderFromStream(ShaderProgram & shaderProgram, std::ifstream & ifs);
    static std::string extractPreprocessorCommand(const std::string & str);

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_SHADERLOADER__

