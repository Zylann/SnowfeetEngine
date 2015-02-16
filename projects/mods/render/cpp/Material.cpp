#include "Material.hpp"

namespace sn {
namespace render {

Material::~Material()
{
    if (m_shader)
        m_shader->release();
}

bool Material::canLoad(const AssetMetadata & meta) const
{
    // TODO Material::canLoad()
    return false;
}

void Material::setShader(ShaderProgram * shader)
{
    SN_ASSERT(shader != nullptr, "Material shader cannot be null");
    if (m_shader)
        m_shader->release();
    m_shader = shader;
    if (m_shader)
        m_shader->addRef();
}


} // namespace render
} // namespace sn

