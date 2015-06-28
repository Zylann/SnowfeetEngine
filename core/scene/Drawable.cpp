#include <core/scene/Scene.hpp>
#include <core/asset/AssetDatabase.hpp>
#include "Drawable.hpp"

namespace sn
{

const std::string Drawable::TAG = "Drawable";

//------------------------------------------------------------------------------
void Drawable::onReady()
{
    Entity3D::onReady();
    addTag(TAG);
}

} // namespace sn


