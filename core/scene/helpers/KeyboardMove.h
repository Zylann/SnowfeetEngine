#ifndef __HEADER_SN_KEYBOARDMOVE__
#define __HEADER_SN_KEYBOARDMOVE__

#include <core/scene/Entity.h>

namespace sn
{

/// \brief Debug helper to move an entity with the keyboard in 3D
class SN_API KeyboardMove : public Entity
{
public:
    SN_OBJECT

    void onReady() override;
    bool onSystemEvent(const Event & event) override;
    void onUpdate() override;

    void serializeState(Variant & o, const SerializationContext & context) override;
    void unserializeState(const Variant & o, const SerializationContext & context) override;

private:
    void onKey(KeyCode key, bool isDown);

private:
    f32 m_speed;
    Vector3f m_motor;
    Vector3f m_angularMotor;

};

} // namespace sn

#endif // __HEADER_SN_KEYBOARDMOVE__

