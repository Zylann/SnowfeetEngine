#include <core/scene/Scene.h>
#include <core/asset/AssetDatabase.h>
#include "MeshEntity.h"
#include "RenderManager.h"

namespace sn
{

SN_OBJECT_IMPL(MeshEntity)

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
void MeshEntity::onDraw(RenderState & state)
{
    const Mesh * mesh = getMesh();
    if (mesh)
    {
        // If the drawable has a material, apply it
        Material * material = getMaterial();
        if (material)
        {
            VideoDriver & driver = state.driver;

            Matrix4 normalMatrix;
            normalMatrix.setRotation(getGlobalRotation());

            if (!m_material.isNull())
            {
                Material & material = *m_material.get();

                driver.setDepthTest(material.isDepthTest());
                driver.setBlendMode(material.getBlendMode());

                ShaderProgram * shader = material.getShader();

                if (shader)
                {
                    driver.useProgram(material.getShader());

                    Matrix4 modelViewMatrix;
                    modelViewMatrix.setByProduct(state.viewMatrix, getGlobalMatrix());

                    // Note: Matrix4 is row-major with translation in the last row
                    shader->setParam(Material::PROJECTION_MATRIX, state.projectionMatrix.values(), false);
                    shader->setParam(Material::MODEL_VIEW_MATRIX, modelViewMatrix.values(), false);
                    shader->setParam(Material::NORMAL_MATRIX, state.normalMatrix.values(), false);

                    material.applyParameters();
                }
            }

            // Draw the mesh
            driver.drawMesh(*mesh);
        }
        //else
        //{
        //    // TODO Draw the mesh with a dummy material
        //}
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

} // namespace sn


