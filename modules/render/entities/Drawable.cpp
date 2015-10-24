#include <core/scene/Scene.h>
#include <core/asset/AssetDatabase.h>
#include "Drawable.h"
#include "RenderManager.h"

namespace sn {
namespace render {

//------------------------------------------------------------------------------
Drawable::~Drawable()
{
}

//------------------------------------------------------------------------------
Drawable::Drawable():
    m_drawOrder(0)
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
void Drawable::onDraw(IDrawContext & dc)
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
void Drawable::serializeState(sn::Variant & o, const SerializationContext & context)
{
    sn::Drawable::serializeState(o, context);

    sn::serialize(o["drawOrder"], m_drawOrder);

    // TODO Upgrade saving of asset references
	
	if (!m_material.isNull())
		sn::serialize(o["material"], m_material.get()->getAssetMetadata().name);

	if (!m_mesh.isNull())
		sn::serialize(o["mesh"], m_mesh.get()->getAssetMetadata().name);

    // TODO
}

//------------------------------------------------------------------------------
void Drawable::unserializeState(const sn::Variant & o, const SerializationContext & context)
{
    sn::Drawable::unserializeState(o, context);

    sn::unserialize(o["drawOrder"], m_drawOrder);

    std::string meshLocation;
    sn::unserialize(o["mesh"], meshLocation);
    m_mesh.set(getAssetBySerializedLocation<Mesh>(         meshLocation,     context.getModule(), this));

    std::string materialLocation;
    sn::unserialize(o["material"], materialLocation);
    m_material.set(getAssetBySerializedLocation<Material>( materialLocation, context.getModule(), this));

    // TODO
}

} // namespace render
} // namespace sn


