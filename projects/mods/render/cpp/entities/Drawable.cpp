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
void Drawable::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity3D::serializeState(o, context);
    sn::serialize(o["drawOrder"], m_drawOrder);

    // TODO Upgrade saving of asset references
	
	if (!m_material.isNull())
		sn::serialize(o["material"], m_material.get()->getAssetMetadata().name);

	if (!m_mesh.isNull())
		sn::serialize(o["mesh"], m_mesh.get()->getAssetMetadata().name);

    // TODO
}

//------------------------------------------------------------------------------
void Drawable::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity3D::unserializeState(o, context);
    sn::unserialize(o["drawOrder"], m_drawOrder);

    m_mesh.set(getAssetBySerializedLocation<Mesh>(         o["mesh"].getString(),     context.getModule(), this));
    m_material.set(getAssetBySerializedLocation<Material>( o["material"].getString(), context.getModule(), this));

    // TODO
}

} // namespace render
} // namespace sn


