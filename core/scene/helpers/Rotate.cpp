#include <core/scene/Scene.hpp>
#include "Rotate.hpp"
#include "../Entity3D.hpp"

namespace sn
{

Rotate::Rotate() : Entity(),
	m_angularSpeed(30, 30, 0)
{
}

void Rotate::onReady()
{
	setUpdatable(true);
}

void Rotate::onUpdate()
{
	Time delta = getScene()->getDeltaTime();

	if (getParent() && getParent()->isInstanceOf<Entity3D>())
	{
		Entity3D * parent3D = (Entity3D*)getParent();
		parent3D->setRotation( parent3D->getRotation() * Quaternion(m_angularSpeed.getEulerAngles() * delta.asSeconds()) );
	}
}

void Rotate::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
	Entity::unserializeState(o, context);
	
	sn::serialize(o["angularSpeed"], m_angularSpeed);
}

void Rotate::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
	Entity::unserializeState(o, context);

	if (o["angularSpeed"].isArray())
		sn::unserialize(o["angularSpeed"], m_angularSpeed);
}

} // namespace sn
