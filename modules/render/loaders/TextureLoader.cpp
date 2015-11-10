#include <core/asset/AssetDatabase.h>

#include <modules/render/Texture.h>

#include "TextureLoader.h"
#include "../Texture.h"

namespace sn {
namespace render {

//-----------------------------------------------------------------------------
const ObjectType & TextureLoader::getBaseAssetType() const
{
    return sn::getObjectType<sn::render::Texture>();
}

//-----------------------------------------------------------------------------
const ObjectType & TextureLoader::getAssetInstanceType() const
{
    return sn::getObjectType<sn::render::Texture>();
}

//-----------------------------------------------------------------------------
bool TextureLoader::canLoad(const AssetMetadata & meta) const
{
    // If the engine can load images, we can load textures out of them
    AssetLoader * imageLoader = AssetDatabase::get().findLoader<Image>();
    return imageLoader ? imageLoader->canLoad(meta) : false;
}

//-----------------------------------------------------------------------------
bool TextureLoader::isDirect(const AssetMetadata & meta) const
{
    const Variant & args = meta.variantData;
    const std::string usage = args["usage"].getString();
    return usage == "texture" || usage == "both";
}

//-----------------------------------------------------------------------------
bool TextureLoader::load(std::ifstream & ifs, Asset & asset) const
{
    Texture * texture = checked_cast<Texture*>(&asset);
    const auto & meta = asset.getAssetMetadata();

    // If the texture has no associated image
    if (texture->getImage() == nullptr)
    {
        // Create image
        Image * img = new Image();
        // Set as source
        texture->setSourceImage(*img);
        // The texture takes ownership of the image
        img->release();

        //img = AssetDatabase::get().getAsset<Image>(meta.module, meta.name);
        //if (img != nullptr)
        //{
        //    texture->setSourceImage(*img);
        //}
    }

    // Load image
    AssetLoader * imageLoader = AssetDatabase::get().findLoader<Image>();
    if (imageLoader)
    {
        if (!imageLoader->load(ifs, *texture->getImage()))
            return false;
    }

    // Set texture flags
    const Variant & metaArgs = meta.variantData;
    texture->setKeepSourceInMemory(metaArgs["keepInMemory"].getBool());
    texture->setSmooth(metaArgs["smooth"].getBool());
    texture->setRepeated(metaArgs["repeated"].getBool());

    return texture->uploadToVRAM();
}

//-----------------------------------------------------------------------------
//s32 TextureLoader::getPriority(const AssetLoader & other) const
//{
//    // If the other loads images, run after this one to be able to get image data
//    if (other.getBaseAssetType() == sn::Image::__sGetObjectType())
//        return 1;
//    // Don't care otherwise
//    return 0;
//}

} // namespace render
} // namespace sn

