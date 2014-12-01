#include "ShaderAssetType.hpp"
#include <core/util/stringutils.hpp>

namespace sn
{

bool ShaderAssetType::canLoad(String path, const AssetMetadata & meta) const
{
    String ext = getFileExtension(path);
    return ext == L"vs" || ext == L"ps" || ext == L".shader";
}

Asset * ShaderAssetType::createInstance()
{
    // TODO ShaderType::createInstance
    return nullptr;
}

} // namespace sn


