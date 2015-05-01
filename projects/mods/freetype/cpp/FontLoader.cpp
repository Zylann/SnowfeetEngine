#include <core/util/stringutils.hpp>
#include "FontLoader.hpp"

using namespace sn;

namespace freetype
{

const sn::ObjectType & FontLoader::getBaseAssetType() const
{
    return sn::Font::__sGetObjectType();
}

bool FontLoader::canLoad(const sn::AssetMetadata & meta) const
{
    String ext = getFileExtension(meta.path);
    return ext == L".ttf";
}

bool FontLoader::load(std::ifstream & ifs, sn::Asset & asset) const
{
    // TODO

    return false;
}

} // namespace freetype

