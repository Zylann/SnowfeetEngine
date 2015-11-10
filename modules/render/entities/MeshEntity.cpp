#include <core/scene/Scene.h>
#include <core/asset/AssetDatabase.h>
#include "MeshEntity.h"
#include "RenderManager.h"

namespace sn {
namespace render {

//------------------------------------------------------------------------------
MeshEntity::~MeshEntity()
{
}

//------------------------------------------------------------------------------
MeshEntity::MeshEntity() : Drawable(),
    m_drawOrder(0)
{
}

//------------------------------------------------------------------------------
void MeshEntity::setMaterial(Material * material)
{
    m_material.set(material);
}

//------------------------------------------------------------------------------
void MeshEntity::setMesh(Mesh * mesh)
{
    m_mesh.set(mesh);
}

//------------------------------------------------------------------------------
void MeshEntity::onDraw(IDrawContext & dc)
{
    const Mesh * mesh = getMesh();
    if (mesh)
    {
        // If the drawable has a material, apply it
        Material * material = getMaterial();
        if (material)
        {
            Matrix4 normalMatrix;
            normalMatrix.setRotation(getGlobalRotation());
            dc.setNormalMatrix(normalMatrix);

            dc.setModelMatrix(getGlobalMatrix());

            dc.setMaterial(*material);
        }

        // Draw the mesh
        dc.drawMesh(*mesh);
    }
}

//------------------------------------------------------------------------------
void MeshEntity::serializeState(sn::Variant & o, const SerializationContext & context)
{
    Drawable::serializeState(o, context);

    sn::serialize(o["drawOrder"], m_drawOrder);

    // TODO Upgrade saving of asset references
	
	if (!m_material.isNull())
		sn::serialize(o["material"], m_material.get()->getAssetMetadata().name);

	if (!m_mesh.isNull())
		sn::serialize(o["mesh"], m_mesh.get()->getAssetMetadata().name);

    // TODO
}

//------------------------------------------------------------------------------
void MeshEntity::unserializeState(const sn::Variant & o, const SerializationContext & context)
{
    Drawable::unserializeState(o, context);

    sn::unserialize(o["drawOrder"], m_drawOrder);

    std::string meshLocation;
    sn::unserialize(o["mesh"], meshLocation);
    if (!meshLocation.empty())
        m_mesh.set(getAssetBySerializedLocation<Mesh>(meshLocation, context.getProject(), true));

    std::string materialLocation;
    sn::unserialize(o["material"], materialLocation);
    if (!materialLocation.empty())
        m_material.set(getAssetBySerializedLocation<Material>(materialLocation, context.getProject(), true));

    // TODO
}

} // namespace render
} // namespace sn


