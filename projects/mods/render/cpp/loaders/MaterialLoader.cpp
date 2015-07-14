#include <core/util/stringutils.h>
#include <core/asset/AssetDatabase.h>

#include "../Material.h"
#include "MaterialLoader.h"

namespace sn {
namespace render {

//------------------------------------------------------------------------------
const ObjectType & MaterialLoader::getBaseAssetType() const
{
    return sn::getObjectType<sn::Material>();
}

//------------------------------------------------------------------------------
const ObjectType & MaterialLoader::getAssetInstanceType() const
{
    return sn::getObjectType<sn::render::Material>();
}

//------------------------------------------------------------------------------
bool MaterialLoader::canLoad(const AssetMetadata & meta) const
{
    String ext = sn::getFileExtension(meta.path);
    // Note: on Windows, sometimes ".mat" is associated to a Microsoft Access database,
    // and the file ends up being shown and acting as a SHORTCUT, due to odd registry changes.
    // This is plain messy. I kindly hate the person who decided this behaviour.
    return ext == L".mat" || ext == L".material";
}

//------------------------------------------------------------------------------
bool MaterialLoader::load(std::ifstream & ifs, Asset & asset) const
{
    sn::render::Material * materialPtr = checked_cast<sn::render::Material*>(&asset);
    sn::render::Material & mat = *materialPtr;

    const AssetMetadata & meta = mat.getAssetMetadata();

    JsonBox::Value doc;
    doc.loadFromStream(ifs);

    // Shader
    mat.setShader(getAssetBySerializedLocation<ShaderProgram>(doc["shader"].getString(), meta.module, &mat));

    // Depth
    bool depthTest = false;
    sn::unserialize(doc["depthTest"], depthTest);
    mat.setDepthTest(depthTest);

    // Blending
    BlendMode blendMode;
    sn::render::unserialize(doc["blend"], blendMode);
    mat.setBlendMode(blendMode);

    // Params
    JsonBox::Value params = doc["params"].getObject();
    if (params.isObject())
    {
        auto paramsObject = params.getObject();
        for (auto it = paramsObject.begin(); it != paramsObject.end(); ++it)
        {
            auto & v = it->second;
            // {"@type":"texture|rendertexture", "value":"foobar"}
            if (v.isObject())
            {
                auto a = it->second.getObject();
                auto typeTag = a[SN_JSON_TYPE_TAG];
                auto valueTag = a["value"];

                if (typeTag.isString() && valueTag.isString())
                {
                    std::string stype = typeTag.getString();
                    std::string loc = valueTag.getString();

                    if (stype == "texture")
                    {
                        Texture * tex = getAssetBySerializedLocation<Texture>(loc, meta.module, &mat);
                        if (tex)
                            mat.setTexture(it->first, tex);
                        else
                            SN_ERROR("Texture not found: " << loc);
                    }
                    else if (stype == "rendertexture")
                    {
                        RenderTexture * rt = getAssetBySerializedLocation<RenderTexture>(loc, meta.module, &mat);
                        if (rt)
                            mat.setRenderTexture(it->first, rt);
                        else
                            SN_ERROR("RenderTexture not found: " << loc);
                    }
                    else
                        SN_ERROR("Unknown specified type: " << stype);
                }
            }
            else if (v.isArray())
            {
                auto & a = it->second.getArray();
                if (a.size() == 2)
                {
                    Vector2f p;
                    sn::unserialize(v, p);
                    mat.setParam(it->first, p.x(), p.y());
                }
                else if (a.size() == 3)
                {
                    Vector3f p;
                    sn::unserialize(v, p);
                    mat.setParam(it->first, p.x(), p.y(), p.z());
                }
                else if (a.size() == 4)
                {
                    Vector4f p;
                    sn::unserialize(v, p);
                    mat.setParam(it->first, p.x(), p.y(), p.z(), p.w());
                }
                // ...
            }
            else if (v.isDouble())
            {
                mat.setParam(it->first, static_cast<f32>(v.getDouble()));
            }
            // TODO Handle other param types
        }
    }

    return mat.getShader() != nullptr;
}

} // namespace render
} // namespace sn

