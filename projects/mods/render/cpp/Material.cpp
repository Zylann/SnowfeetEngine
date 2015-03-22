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
    return ext == L".mat";
}

//------------------------------------------------------------------------------
bool Material::loadFromStream(std::ifstream & ifs)
{
    JsonBox::Value doc;
    doc.loadFromStream(ifs);
    
    std::string shaderName;
    sn::unserialize(doc["shader"], shaderName);

    ShaderProgram * shader = AssetDatabase::get().getAsset<ShaderProgram>(getAssetMetadata().module, shaderName);
    if (shader)
    {
        setShader(shader);
        return true;
    }
    else
    {
        SN_ERROR("Material shader not found: '" << shaderName << "'");
        return false;
    }

    sn::unserialize(doc["depthTest"], m_depthTest);
}

//------------------------------------------------------------------------------
void Material::setShader(ShaderProgram * shader)
{
    SN_ASSERT(shader != nullptr, "Material shader cannot be null");
	m_shader.set(shader);
}


} // namespace render
} // namespace sn

