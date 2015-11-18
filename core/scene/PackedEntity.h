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

    /// \brief Gets the type of the root entity, if any.
    /// \return Entity type, or null if none is defined.
    const ObjectType * getRootType() const;

    /// \brief Helper testing if the root entity is equal or derived from Entity_T.
    template <class Entity_T>
    bool isRootOfType()
    {
        const ObjectType * rootType = getRootType();
        if (rootType)
            return rootType->is(sn::getObjectType<Entity_T>(), true);
        return false;
    }

    void instantiate(
        Entity & a_parent, 
        const std::string & contextModuleName, 
        std::vector<Entity*> * out_rootEntities = nullptr
    );

protected:
    ObjectDB * getFromAssetDatabase(const std::string & location) const override;

private:
    void instantiateOnly(
        Entity & a_parent, 
        const std::string & contextModuleName, 
        std::vector<Entity*> * out_rootEntities
    ) const;

};

} // namespace sn

#endif //__HEADER_SN_PACKEDENTITY__

