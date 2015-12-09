#include <core/util/stringutils.h>
#include <core/util/typecheck.h>

#include <modules/render/Mesh.h>

#include "BasicMeshLoader.h"
#include "ply/PLYLoader.h"

namespace sn
{

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

} // namespace sn

