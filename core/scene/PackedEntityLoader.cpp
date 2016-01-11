#include "PackedEntityLoader.h"
#include "PackedEntity.h"
#include "../util/stringutils.h"
#include "../util/typecheck.h"

namespace sn
{

SN_OBJECT_IMPL(PackedEntityLoader)

//------------------------------------------------------------------------------
const ObjectType & PackedEntityLoader::getBaseAssetType() const
{
    return sn::getObjectType<PackedEntity>();
}

//------------------------------------------------------------------------------
bool PackedEntityLoader::canLoad(const AssetMetadata & meta) const
{
    String ext = getFileExtension(meta.path);
    return ext == L".scene" || ext == L".entity";
}

//------------------------------------------------------------------------------
bool PackedEntityLoader::load(std::ifstream & ifs, Asset & asset) const
{
    sn::PackedEntity * packedEntity = checked_cast<PackedEntity*>(&asset);
    SN_ASSERT(packedEntity != nullptr, "PackedEntity type to load mismatches");

    return packedEntity->loadFromStream(ifs);
}

} // namespace sn

