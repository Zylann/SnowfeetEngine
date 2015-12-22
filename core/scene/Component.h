#ifndef __HEADER_SN_COMPONENT__
#define __HEADER_SN_COMPONENT__

#include <bitset>
#include <core/app/ScriptableObject.h>
#include <core/asset/SerializationContext.h>

namespace sn
{

class Entity;

// Base class for components
class SN_API Component : public ScriptableObject
{
public:
    SN_OBJECT

    enum Flag
    {
        FLAG_ENABLED = 0,
        //FLAG_DESTROYED = 1,
        FLAG_COUNT = 8
    };

    Component();

    Entity & getEntity();
    void setEntity(Entity & e);

    bool getFlag(Flag f) const { return m_flags[f]; }

    virtual void serializeState(Variant & o, const SerializationContext & context);
    virtual void unserializeState(const Variant & o, const SerializationContext & context);

protected:
    virtual ~Component();

private:
    Entity * r_entity;
    std::bitset<FLAG_COUNT> m_flags;

};

} // namespace sn

#endif // __HEADER_SN_COMPONENT__


