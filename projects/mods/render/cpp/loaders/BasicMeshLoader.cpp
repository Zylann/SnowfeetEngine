#include <core/asset/base/Mesh.h>
#include <core/util/stringutils.h>
#include <core/util/typecheck.h>

#include "BasicMeshLoader.hpp"
#include "ply/PLYLoader.hpp"

namespace sn {
namespace render {

//-----------------------------------------------------------------------------
const ObjectType & BasicMeshLoader::getBaseAssetType() const
{
    return sn::getObjectType<Mesh>();
}

//-----------------------------------------------------------------------------
bool BasicMeshLoader::canLoad(const AssetMetadata & meta) const
{
    String ext = sn::getFileExtension(meta.path);
    return ext == L".ply";
}

//-----------------------------------------------------------------------------
bool BasicMeshLoader::load(std::ifstream & ifs, Asset & asset) const
{
    Mesh * mesh = checked_cast<Mesh*>(&asset);
    PLYLoader loader(ifs);
    bool success = loader.loadMesh(*mesh);
    mesh->recalculateBounds();
    return success;
}

} // namespace render
} // namespace sn

