#include <core/util/stringutils.h>
#include <core/util/typecheck.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Color.h>

#include "Material.h"

namespace sn {
namespace render {

//------------------------------------------------------------------------------
const char * Material::MAIN_TEXTURE = "u_MainTexture";
const char * Material::MODEL_MATRIX = "u_ModelMatrix";
const char * Material::MODEL_VIEW_MATRIX = "u_ModelView";
const char * Material::VIEW_MATRIX = "u_ViewMatrix";
const char * Material::PROJECTION_MATRIX = "u_Projection";
const char * Material::NORMAL_MATRIX = "u_NormalMatrix";
const char * Material::COLOR = "u_Color";
const char * Material::TIME = "u_Time";

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
bool Material::getParam(const std::string & name, f32 & out_v)
{
    auto it = m_floats.find(name);
    if (it != m_floats.end())
    {
        out_v = it->second;
        return false;
    }
    return false;
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
void Material::setParam(const std::string & name, f32 matrix4x4Values[16])
{
    m_mat4[name] = Matrix4(matrix4x4Values);
}

//------------------------------------------------------------------------------
void Material::setTexture(const std::string & name, Texture * tex)
{
    m_textures[name].set(tex);
}

//------------------------------------------------------------------------------
Texture * Material::getTexture(const std::string & name) const
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
        return it->second.get();
    return nullptr;
}

//------------------------------------------------------------------------------
void Material::setRenderTexture(const std::string & name, RenderTexture * tex)
{
    setTexture(name, tex ? tex->getTexture() : nullptr);
}

//------------------------------------------------------------------------------
void Material::applyParameters()
{
    if (m_shader.isNull())
        return;

    ShaderProgram & shader = *m_shader.get();

    // Set textures
    s32 textureUnit = 0;
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
    {
        Texture * tex = it->second.get();
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

