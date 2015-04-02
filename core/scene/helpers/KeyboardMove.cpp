#include "KeyboardMove.hpp"
#include "../Entity3D.hpp"
#include "../Scene.hpp"

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
    float v = isDown ? 1.f : 0.f;

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

    default:
        break;
    }
}

void KeyboardMove::onUpdate()
{
    if (getParent() && getParent()->isInstanceOf<Entity3D>())
    {
        Entity3D & parent = *(Entity3D*)getParent();
        Time dt = getScene()->getDeltaTime();
        parent.setGlobalPosition(parent.getPosition() + m_motor * m_speed * dt.asSeconds());
    }
}

void KeyboardMove::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity::serializeState(o, context);
    sn::serialize(o["speed"], m_speed);
}

void KeyboardMove::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity::unserializeState(o, context);
    sn::unserialize(o["speed"], m_speed, 1.f);
}

} // namespace sn

