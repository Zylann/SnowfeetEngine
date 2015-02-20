#include "Material.hpp"

namespace sn {
namespace render {

Material::~Material()
{
}

bool Material::canLoad(const AssetMetadata & meta) const
{
    // TODO Material::canLoad()
    return false;
}

void Material::setShader(ShaderProgram * shader)
{
    SN_ASSERT(shader != nullptr, "Material shader cannot be null");
	m_shader.set(shader);
}


} // namespace render
} // namespace sn

