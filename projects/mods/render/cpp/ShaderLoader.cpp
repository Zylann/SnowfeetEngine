#include "ShaderLoader.hpp"
#include <core/util/stringutils.hpp>

namespace sn
{

bool ShaderLoader::canLoad(String path, const AssetMetadata & meta) const
{
    String ext = getFileExtension(path);
    return ext == L"vs" || ext == L"ps" || ext == L".shader";
}

IAsset * ShaderLoader::load(String path, const AssetMetadata & meta, f32 & out_progress)
{
    // TODO ShaderLoader::load
    return nullptr;
}

} // namespace sn


