#ifndef __HEADER_SN_SQUIRRELCOMPONENT__
#define __HEADER_SN_SQUIRRELCOMPONENT__

#include <core/scene/Component.h>
#include <core/squirrel/Instance.h>

namespace sn
{

class SN_API SquirrelComponent : public Component
{
public:
    SN_OBJECT

    SquirrelComponent();

    squirrel::Instance & getInstance() { return m_instance; }

    void serializeState(Variant & o, const SerializationContext & context) override;
    void unserializeState(const Variant & o, const SerializationContext & context) override;

private:
    ~SquirrelComponent();

    squirrel::Instance m_instance;
};

} // namespace sn

#endif // __HEADER_SN_SQUIRRELCOMPONENT__

