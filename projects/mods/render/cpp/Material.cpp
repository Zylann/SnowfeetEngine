#include <core/util/stringutils.hpp>
#include "Material.hpp"

#include <core/asset/AssetDatabase.hpp>

namespace sn {
namespace render {

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
bool Material::canLoad(const AssetMetadata & meta) const
{
    String ext = sn::getFileExtension(meta.path);
	// Note: on Windows, sometimes ".mat" is associated to an Microsoft Access database,
	// and the file ends up being shown and acting as a SHORTCUT, due to odd registry changes.
	// This is plain messy. I kindly hate the person who decided this behaviour.
    return ext == L".mat" || ext == L".material";
}

//------------------------------------------------------------------------------
bool Material::loadFromStream(std::ifstream & ifs)
{
    const AssetMetadata & meta = getAssetMetadata();

    JsonBox::Value doc;
    doc.loadFromStream(ifs);
    
    setShader(getAssetBySerializedLocation<ShaderProgram>(doc["shader"].getString(), meta.module, this));
    sn::unserialize(doc["depthTest"], m_depthTest);

    JsonBox::Value params = doc["params"].getObject();
    if (params.isObject())
    {
        auto o = params.getObject();
        for (auto it = o.begin(); it != o.end(); ++it)
        {
            // {"@type":"texture|rendertexture", "value":"foobar"}
            if (it->second.isObject())
            {
                auto a = it->second.getObject();
                auto typeTag = a[SN_JSON_TYPE_TAG];
                auto valueTag = a["value"];

                if (!typeTag.isString() && !valueTag.isString())
                {
                    std::string stype = typeTag.getString();
                    std::string loc = valueTag.getString();

                    if (stype == "texture")
                        setParam(it->first, getAssetBySerializedLocation<Texture>(loc, meta.module, this));
                    else if (stype == "rendertexture")
                        setParam(it->first, getAssetBySerializedLocation<RenderTexture>(loc, meta.module, this));
                    else
                        SN_ERROR("Unknown specified type: " << stype);
                }
            }
            //else
            //{
            //    // ...
            //}
            // TODO Handle other param types
        }
    }

    return !m_shader.isNull();
}

//------------------------------------------------------------------------------
void Material::setShader(ShaderProgram * shader)
{
    SN_ASSERT(shader != nullptr, "Material shader cannot be null (" << sn::toString(getAssetMetadata().path) << ")");
	m_shader.set(shader);
}

//------------------------------------------------------------------------------
void Material::setParam(const std::string & name, Texture * texture)
{
    m_textures[name].set(texture);
}

//------------------------------------------------------------------------------
void Material::setParam(const std::string & name, RenderTexture * tex)
{
    setParam(name, tex ? tex->getTexture() : nullptr);
}

//------------------------------------------------------------------------------
void Material::apply()
{
    if (m_shader.isNull())
        return;

    ShaderProgram & shader = *m_shader.get();

    // Set textures
    s32 textureUnit = 0;
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
    {
        Texture * tex = it->second.get();
        Texture::setActive(textureUnit, tex);
        shader.setParam(it->first, textureUnit);
        ++textureUnit;
    }
}

} // namespace render
} // namespace sn

