#include "KeyboardMove.h"
#include "../Entity3D.h"
#include "../Scene.h"

namespace sn
{

void KeyboardMove::onReady()
{
    listenToSystemEvents();
    setUpdatable(true);
}

bool KeyboardMove::onSystemEvent(const Event & event)
{
    switch (event.type)
    {
    case SN_EVENT_KEY_DOWN:
        onKey(event.keyboard.keyCode, true);
        break;

    case SN_EVENT_KEY_UP:
        onKey(event.keyboard.keyCode, false);
        break;

    default:
        break;
    }

    return false;
}

void KeyboardMove::onKey(KeyCode key, bool isDown)
{
    float v = (isDown ? 1.f : 0.f);
    float a = v * 30.f;

    switch (key)
    {
    case SN_KEY_LEFT:
    case SN_KEY_Q:
    case SN_KEY_A:
        m_motor.x() = -v;
        break;

    case SN_KEY_RIGHT:
    case SN_KEY_D:
        m_motor.x() = v;
        break;

    case SN_KEY_UP:
    case SN_KEY_Z:
    case SN_KEY_W:
        m_motor.z() = v;
        break;

    case SN_KEY_DOWN:
    case SN_KEY_S:
        m_motor.z() = -v;
        break;

    case SN_KEY_SPACE:
        m_motor.y() = v;
        break;

    case SN_KEY_LSHIFT:
        m_motor.y() = -v;
        break;

    case SN_KEY_PAD4:
        m_angularMotor.y() = -a;
        break;

    case SN_KEY_PAD6:
        m_angularMotor.y() = a;
        break;

    case SN_KEY_PAD8:
        m_angularMotor.x() = -a;
        break;

    case SN_KEY_PAD2:
        m_angularMotor.x() = a;
        break;

    case SN_KEY_PAD7:
        m_angularMotor.z() = a;
        break;

    case SN_KEY_PAD9:
        m_angularMotor.z() = -a;
        break;

    default:
        break;
    }
}

void KeyboardMove::onUpdate()
{
    if (getParent() && getParent()->isInstanceOf<Entity3D>())
    {
        Time dt = getScene()->getDeltaTime();
        Entity3D & parent = *(Entity3D*)getParent();

        Quaternion parentRotation = parent.getRotation();

        Vector3f motor = parentRotation * m_motor;

        parent.setPosition(parent.getPosition() + motor * m_speed * dt.asSeconds());

        Quaternion deltaRot(m_angularMotor * dt.asSeconds());
        parent.setRotation( (parentRotation * deltaRot).normalize() );

        // Change interpupillar distance when using VR
        //f32 eyeDelta = 0;
        //if (isKeyPressed(SN_KEY_PAD1))
        //    eyeDelta = -1.f * dt.asSeconds();
        //else if (isKeyPressed(SN_KEY_PAD3))
        //    eyeDelta = 1.f * dt.asSeconds();
        //for (u32 i = 0; i < parent.getChildCount(); ++i)
        //{
        //    Entity * child = parent.getChildByIndex(i);
        //    if (child->isInstanceOf<Entity3D>())
        //    {
        //        Entity3D & child3D = *(Entity3D*)child;
        //        if (child3D.hasTag("VR_LeftEye"))
        //            child3D.setPosition(child3D.getPosition() + Vector3f(-eyeDelta, 0, 0));
        //        else if (child3D.hasTag("VR_RightEye"))
        //            child3D.setPosition(child3D.getPosition() + Vector3f(eyeDelta, 0, 0));
        //    }
        //}
        //SN_LOG("Pos: " << sn::toString(parent.getGlobalPosition()));// << ", up: " << sn::toString(up));
        //SN_LOG("Forward: " << sn::toString(parent.getForwardVector()));// << ", up: " << sn::toString(up));
        //SN_LOG("rotation: " << sn::toString(parent.getGlobalRotation()));
    }
}

void KeyboardMove::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity::serializeState(o, context);
    sn::serialize(o["speed"], m_speed);
}

void KeyboardMove::unserializeState(const Variant & o, const SerializationContext & context)
{
    Entity::unserializeState(o, context);
    sn::unserialize(o["speed"], m_speed, 1.f);
}

} // namespace sn

