#include <core/scene/Scene.hpp>
#include "Drawable.hpp"
#include "RenderManager.hpp"

namespace sn {
namespace render {

const std::string Drawable::TAG = "Drawable";

Drawable::~Drawable()
{
}

void Drawable::setMaterial(Material * material)
{
    m_material.set(material);
}

Drawable::Drawable()
{
    m_mesh.set(new Mesh());
}

void Drawable::onReady()
{
    addTag(TAG);
}

} // namespace render
} // namespace sn


