#ifndef __HEADER_SN_PACKEDENTITY__
#define __HEADER_SN_PACKEDENTITY__

#include <core/asset/ObjectDB.h>
#include <core/scene/Entity.h>
#include <iostream>

namespace sn
{
    
/// \brief Serialized entity/array of entities that can be saved to a file or instantiated in a scene.
class PackedEntity : public ObjectDB
{
public:
    SN_SCRIPT_OBJECT(sn::PackedEntity, sn::ObjectDB)

    static const char * CHILDREN_TAG;

    bool loadFromVariant(Variant & doc) override;
    bool loadFromLegacyDump(Variant & doc);

    void loadFromInstance(const Entity & entity);

    void instantiate(Entity & a_parent, const std::string & contextModuleName) const;

protected:
    ObjectDB * getFromAssetDatabase(const std::string & location) const override;

};

} // namespace sn

#endif //__HEADER_SN_PACKEDENTITY__

