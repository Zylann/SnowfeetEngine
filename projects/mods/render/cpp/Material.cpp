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
    JsonBox::Value doc;
    doc.loadFromStream(ifs);
    
    setShader(getAssetBySerializedLocation<ShaderProgram>(doc["shader"].getString(), getAssetMetadata().module, this));
    sn::unserialize(doc["depthTest"], m_depthTest);

    return !m_shader.isNull();
}

//------------------------------------------------------------------------------
void Material::setShader(ShaderProgram * shader)
{
    SN_ASSERT(shader != nullptr, "Material shader cannot be null");
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

