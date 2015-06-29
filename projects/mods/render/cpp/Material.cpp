#include <core/util/stringutils.h>
#include <core/util/typecheck.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Color.h>

#include "Material.hpp"

namespace sn {
namespace render {

//------------------------------------------------------------------------------
Material::~Material()
{
}

//------------------------------------------------------------------------------
void Material::setShader(ShaderProgram * shader)
{
    SN_ASSERT(shader != nullptr, "Material shader cannot be null (" << sn::toString(getAssetMetadata().path) << ")");
	m_shader.set(shader);
}

//------------------------------------------------------------------------------
void Material::setRenderTexture(const std::string & name, RenderTexture * tex)
{
    setTexture(name, tex ? tex->getTexture() : nullptr);
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
        TextureBase * tex = it->second.get();
        Texture::setActive(textureUnit, sn::checked_cast<Texture*>(tex));
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

    // Set vec3 params
    for (auto it = m_vec3.begin(); it != m_vec3.end(); ++it)
    {
        Vector3f v = it->second;
        shader.setParam(it->first, v[0], v[1], v[2]);
    }

    // Set vec4 params
    for (auto it = m_vec4.begin(); it != m_vec4.end(); ++it)
    {
        Vector4f v = it->second;
        shader.setParam(it->first, v[0], v[1], v[2], v[3]);
    }

    // Set matrix params
    for (auto it = m_mat4.begin(); it != m_mat4.end(); ++it)
    {
        const Matrix4 & m = it->second;
        shader.setParam(it->first, m.values());
    }
}

} // namespace render
} // namespace sn

