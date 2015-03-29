#include "Material.hpp"

namespace sn
{

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

void Material::setParam(const std::string & name, f32 matrix4x4Values[16])
{
    m_mat4[name] = Matrix4(matrix4x4Values);
}

} // namespace sn

