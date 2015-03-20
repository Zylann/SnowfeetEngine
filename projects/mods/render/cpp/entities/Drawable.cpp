#include <core/scene/Scene.hpp>
#include <core/asset/AssetDatabase.hpp>
#include "Drawable.hpp"
#include "RenderManager.hpp"

namespace sn {
namespace render {

const std::string Drawable::TAG = "Drawable";

//------------------------------------------------------------------------------
Drawable::~Drawable()
{
}

//------------------------------------------------------------------------------
void Drawable::setMaterial(Material * material)
{
    m_material.set(material);
}

//------------------------------------------------------------------------------
void Drawable::setMesh(Mesh * mesh)
{
    m_mesh.set(mesh);
}

//------------------------------------------------------------------------------
Drawable::Drawable()
{
    m_mesh.set(new Mesh());
}

//------------------------------------------------------------------------------
void Drawable::onReady()
{
    addTag(TAG);
}

//------------------------------------------------------------------------------
void Drawable::serializeState(JsonBox::Value & o)
{
    Entity3D::serializeState(o);
    sn::serialize(o["drawOrder"], m_drawOrder);
	
	if (!m_material.isNull())
		sn::serialize(o["material"], m_material.get()->getAssetMetadata().name);

	if (!m_mesh.isNull())
		sn::serialize(o["mesh"], m_mesh.get()->getAssetMetadata().name);

    // TODO
}

//------------------------------------------------------------------------------
void Drawable::unserializeState(JsonBox::Value & o)
{
    Entity3D::unserializeState(o);
    sn::unserialize(o["drawOrder"], m_drawOrder);

	std::string materialID;
	sn::unserialize(o["material"], materialID);
	if (!materialID.empty())
	{
		// TODO Don't hardcode the name of the module!
		Material * mat = AssetDatabase::get().getAsset<Material>("render", materialID);
		if (mat)
		{
			m_material.set(mat);
		}
	}

	std::string meshID;
	sn::unserialize(o["mesh"], meshID);
	if (!meshID.empty())
	{
		// TODO Don't hardcode the name of the module!
		Mesh * mesh = AssetDatabase::get().getAsset<Mesh>("render", meshID);
		if (mesh)
		{
			m_mesh.set(mesh);
		}
	}

    // TODO
}

} // namespace render
} // namespace sn


