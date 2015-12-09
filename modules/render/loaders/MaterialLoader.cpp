#include <core/util/stringutils.h>
#include <core/asset/AssetDatabase.h>
#include <core/sml/SmlParser.h>

#include "../Material.h"
#include "MaterialLoader.h"

namespace sn
{

//------------------------------------------------------------------------------
const ObjectType & MaterialLoader::getBaseAssetType() const
{
    return sn::getObjectType<Material>();
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
    sn::Material * materialPtr = checked_cast<sn::Material*>(&asset);
    sn::Material & mat = *materialPtr;

    SmlParser parser;
    Variant doc;
    parser.parseValue(ifs, doc);

    return loadFromVariant(doc, mat);
}

//------------------------------------------------------------------------------
bool MaterialLoader::loadFromVariant(const sn::Variant & doc, sn::Material & mat) const
{
    const AssetMetadata & meta = mat.getAssetMetadata();

    // Shader
    ShaderProgram * shader = getAssetBySerializedLocation<ShaderProgram>(doc["shader"].getString(), meta.project, true);
    if (shader)
        mat.setShader(shader);

    // Depth
    bool depthTest = false;
    sn::unserialize(doc["depthTest"], depthTest);
    mat.setDepthTest(depthTest);

    // Blending
    BlendMode blendMode;
    sn::unserialize(doc["blend"], blendMode);
    mat.setBlendMode(blendMode);

    // Params
    const Variant::Dictionary params = doc["params"].getDictionary();
    if (!params.empty())
    {
        for (auto it = params.begin(); it != params.end(); ++it)
        {
            auto & v = it->second;
            // {"@type":"texture|rendertexture", "value":"foobar"}
            if (v.isDictionary())
            {
                auto a = it->second.getDictionary();
                auto typeTag = a["@type"];
                auto valueTag = a["value"];

                if (typeTag.isString() && valueTag.isString())
                {
                    std::string stype = typeTag.getString();
                    std::string loc = valueTag.getString();

                    if (stype == "texture")
                    {
                        Texture * tex = getAssetBySerializedLocation<Texture>(loc, meta.project, true);
                        if (tex)
                            mat.setTexture(it->first, tex);
                        else
                            SN_ERROR("Texture not found: " << loc);
                    }
                    else if (stype == "rendertexture")
                    {
                        RenderTexture * rt = getAssetBySerializedLocation<RenderTexture>(loc, meta.project, true);
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
            else if (v.isFloat())
            {
                mat.setParam(it->first, static_cast<f32>(v.getFloat()));
            }
            // TODO Handle other param types
        }
    }

    return mat.getShader() != nullptr;
}

} // namespace sn

