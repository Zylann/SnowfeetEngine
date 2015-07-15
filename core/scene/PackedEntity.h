#ifndef __HEADER_SN_PACKEDENTITY__
#define __HEADER_SN_PACKEDENTITY__

#include <core/asset/Asset.h>
#include <core/scene/Entity.h>
#include <iostream>

namespace sn
{
    
/// \brief Serialized entity/array of entities that can be saved to a file or instantiated in a scene.
class PackedEntity : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::PackedEntity, sn::Asset)

    bool loadFromStream(std::istream & is);
    void loadFromInstance(const Entity & entity);

    bool saveToStream(std::ostream & os);

    void instantiate(Entity & a_parent, const SerializationContext & context);

    /// \brief Deserializes an entity from a JSON node.
    /// \param o: JSON node to read as an entity
    /// \param e: parent entity where the deserialized entity has to be placed as child
    /// \param context: context.
    /// \return New deserialized entity, or null if an error occurred.
    static Entity * unserialize(JsonBox::Value & o, Entity * parent, const SerializationContext & context);

    /// \brief Serializes an entity inside a JSON node.
    /// \param o: JSON node to fill
    /// \param e: entity to serialize
    /// \param context: context.
    static void serialize(JsonBox::Value & o, Entity & e, const SerializationContext & context);

private:
    JsonBox::Value m_jsonData;

};

} // namespace sn

#endif //__HEADER_SN_PACKEDENTITY__

