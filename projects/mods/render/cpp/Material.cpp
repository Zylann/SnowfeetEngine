#include <core/util/stringutils.hpp>
#include <core/math/Vector2.hpp>
#include <core/math/Vector3.hpp>
#include <core/math/Color.hpp>

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
            else if (v.isArray())
            {
                auto & a = it->second.getArray();
                if (a.size() == 2)
                {
                    f32 x = sn::unserializeFloat(v[(size_t)0]);
                    f32 y = sn::unserializeFloat(v[1]);
                    setParam(it->first, x, y);
                }
                else if (a.size() == 3)
                {
                    f32 x = sn::unserializeFloat(v[(size_t)0]);
                    f32 y = sn::unserializeFloat(v[1]);
                    f32 z = sn::unserializeFloat(v[2]);
                    setParam(it->first, x, y, z);
                }
                else if (a.size() == 4)
                {
                    f32 x = sn::unserializeFloat(v[(size_t)0]);
                    f32 y = sn::unserializeFloat(v[1]);
                    f32 z = sn::unserializeFloat(v[2]);
                    f32 w = sn::unserializeFloat(v[3]);
                    setParam(it->first, x, y, z, w);
                }
                // ...
            }
            else if (v.isDouble())
            {
                setParam(it->first, v.getDouble());
            }
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
void Material::setParam(const std::string & name, f32 x)
{
    m_floats[name] = x;
}

//------------------------------------------------------------------------------
void Material::setParam(const std::string & name, f32 x, f32 y)
{
    m_vec2[name] = Vector2f(x, y);
}

//------------------------------------------------------------------------------
void Material::setParam(const std::string & name, f32 x, f32 y, f32 z)
{
    m_vec3[name] = Vector3f(x, y, z);
}

//------------------------------------------------------------------------------
void Material::setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w)
{
    m_vec4[name] = Vector4f(x, y, z, w);
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

    // Set float params
    for (auto it = m_floats.begin(); it != m_floats.end(); ++it)
    {
        shader.setParam(it->first, it->second);
    }

    // Set vec2 params
    for (auto it = m_vec2.begin(); it != m_vec2.end(); ++it)
    {
        Vector2f v = it->second;
        shader.setParam(it->first, v[0], v[1]);
    }

    // Set vec4 params
    for (auto it = m_vec4.begin(); it != m_vec4.end(); ++it)
    {
        Vector4f v = it->second;
        shader.setParam(it->first, v[0], v[1], v[2], v[3]);
    }
}

} // namespace render
} // namespace sn

