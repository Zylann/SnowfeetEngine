#include <core/scene/Scene.hpp>
#include "Drawable.hpp"
#include "RenderManager.hpp"

namespace sn {
namespace render {

const std::string Drawable::TAG = "Drawable";

Drawable::~Drawable()
{
    if (m_material)
        m_material->release();
}

void Drawable::setMaterial(Material * material)
{
    if (m_material)
        m_material->release();
    m_material = material;
    if (m_material)
        m_material->addRef();
}

Drawable::Drawable()
{
    m_mesh = new Mesh();
}

void Drawable::onReady()
{
    addTag(TAG);
}

} // namespace render
} // namespace sn


