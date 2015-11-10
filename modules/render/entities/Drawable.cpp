#include <core/scene/Scene.h>
#include <core/asset/AssetDatabase.h>
#include "Drawable.h"

namespace sn {
namespace render {

const std::string Drawable::TAG = "Drawable";

//------------------------------------------------------------------------------
void Drawable::onReady()
{
    Entity3D::onReady();
    addTag(TAG);
}

} // namespace render
} // namespace sn


