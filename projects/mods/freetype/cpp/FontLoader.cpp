#include <core/util/stringutils.hpp>
#include <core/util/typecheck.hpp>

#include "Font.hpp"
#include "FontLoader.hpp"

using namespace sn;

namespace freetype
{

FontLoader::FontLoader():
    m_library(nullptr)
{
    // Initialize Freetype
    if (FT_Init_FreeType(&m_library) != 0)
    {
        SN_ERROR("Failed to initialize FreeType library");
    }
}

FontLoader::~FontLoader()
{
    if (m_library != 0)
    {
        // Deinitialize Freetype
        FT_Done_FreeType(m_library);
    }
}

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
    freetype::Font * font = sn::checked_cast<freetype::Font*>(&asset);
    SN_ASSERT(font != nullptr, "Invalid asset type");

    //AssetMetadata meta = asset.getAssetMetadata();

    //std::vector<u32> sizesToLoad;
    //sn::unserialize(meta.json["sizes"], sizesToLoad);

    // Read the whole stream
    ifs.seekg(0, ifs.end);
    u32 len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char * data = new char[len];
    ifs.read(data, len);

    // Load the face
    FT_Face face;
    if (FT_New_Memory_Face(m_library, reinterpret_cast<const FT_Byte*>(data), len, 0, &face) != 0)
    {
        SN_ERROR("Failed to create Freetype font face from memory");
        delete[] data;
        return false;
    }
    delete[] data;

    // Select the unicode character map
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
    {
        SN_ERROR("Failed to select the Unicode character set (Freetype)");
        return false;
    }

    // Store the loaded font
    font->setFace(face);

    return true;
}

} // namespace freetype

