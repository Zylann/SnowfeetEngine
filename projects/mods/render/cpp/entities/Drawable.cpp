#include <core/scene/Scene.hpp>
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
    // TODO
}

//------------------------------------------------------------------------------
void Drawable::unserializeState(JsonBox::Value & o)
{
    Entity3D::unserializeState(o);
    sn::unserialize(o["drawOrder"], m_drawOrder);
    // TODO
}

} // namespace render
} // namespace sn


