#include "Entity3D.hpp"

namespace sn
{

Entity3D::Entity3D() : Entity(), 
    m_localMatrixNeedUpdate(true), 
    m_globalMatrixNeedUpdate(true),
    m_scale(1,1,1)
{
}

Vector3f Entity3D::getPosition()
{
    return m_position;
}

void Entity3D::setPosition(const Vector3f & newPos)
{
    m_position = newPos;
    m_localMatrixNeedUpdate = true;
    m_globalMatrixNeedUpdate = true;
    onPositionChanged();
}

Quaternion Entity3D::getRotation()
{
    return m_rotation;
}

void Entity3D::setRotation(const Quaternion & newRotation)
{
    m_rotation = newRotation;
    m_localMatrixNeedUpdate = true;
    m_globalMatrixNeedUpdate = true;
    onRotationChanged();
}

Vector3f Entity3D::getScale()
{
    return m_scale;
}

void Entity3D::setScale(const Vector3f & newScale)
{
    m_scale = newScale;
    m_localMatrixNeedUpdate = true;
    m_globalMatrixNeedUpdate = true;
    onScaleChanged();
}

Vector3f Entity3D::getGlobalPosition()
{
	if (getParent() && getParent()->isInstanceOf<Entity3D>())
	{
		Entity3D & parent3D = *(Entity3D*)getParent();
		return parent3D.getGlobalMatrix().transformPoint(m_position);
	}
	else
	{
		return m_position;
	}
}

void Entity3D::setGlobalPosition(const Vector3f & newPos)
{
	if (getParent() && getParent()->isInstanceOf<Entity3D>())
	{
		Entity3D & parent3D = *(Entity3D*)getParent();
		Matrix4 inv;
		if (parent3D.getGlobalMatrix().getInverse(inv))
		{
			m_position = inv.transformPoint(newPos);
		}
		else
		{
			m_position = newPos;
		}
	}
	else
	{
		m_position = newPos;
	}
}

Quaternion Entity3D::getGlobalRotation()
{
	if (getParent() && getParent()->isInstanceOf<Entity3D>())
	{
		Entity3D & parent3D = *(Entity3D*)getParent();
		return m_rotation * parent3D.getRotation();
	}
	else
	{
		return m_rotation;
	}
}

void Entity3D::setGlobalRotation(const Quaternion & newRotation)
{
	if (getParent() && getParent()->isInstanceOf<Entity3D>())
	{
		Entity3D & parent3D = *(Entity3D*)getParent();
		m_rotation = newRotation * parent3D.getRotation().getInverse();
	}
	else
	{
		m_rotation = newRotation;
	}
}

Vector3f Entity3D::getGlobalScale()
{
	if (getParent() && getParent()->isInstanceOf<Entity3D>())
	{
		return m_scale * ((Entity3D*)getParent())->getGlobalScale();
	}
	else
	{
		return m_scale;
	}
}

void Entity3D::setGlobalScale(const Vector3f & newScale)
{
    if (getParent() && getParent()->isInstanceOf<Entity3D>())
    {
        Vector3f parentScale = ((Entity3D*)getParent())->getGlobalScale();
        setScale(newScale * parentScale);
    }
    else
    {
        return setScale(newScale);
    }
}

const Matrix4 & Entity3D::getLocalMatrix() const
{
    if (m_localMatrixNeedUpdate)
    {
        m_localMatrix.loadIdentity();
        m_localMatrix.setRotation(m_rotation);
        m_localMatrix.setTranslation(m_position);
        m_localMatrix.scaleTransform(m_scale);

        m_localMatrixNeedUpdate = false;
    }

    return m_localMatrix;
}

const Matrix4 & Entity3D::getGlobalMatrix() const
{
    if (m_globalMatrixNeedUpdate)
    {
        if (getParent() && getParent()->isInstanceOf<Entity3D>())
        {
            Entity3D * parent3D = (Entity3D*)getParent();
            m_globalMatrix.setByProduct(parent3D->getGlobalMatrix(), getLocalMatrix());
        }
        else
        {
            m_globalMatrix = getLocalMatrix();
        }

        m_globalMatrixNeedUpdate = false;
    }

    return m_globalMatrix;
}

void Entity3D::updateChildrenTransform()
{
    for (u32 i = 0; i < getChildCount(); ++i)
    {
        Entity * e = getChildByIndex(i);
        if (e->isInstanceOf<Entity3D>())
        {
            Entity3D * e3D = (Entity3D*)e;
            e3D->m_globalMatrixNeedUpdate = true;
            e3D->updateChildrenTransform();
        }
    }
}

void Entity3D::onPositionChanged()
{
    updateChildrenTransform();
}

void Entity3D::onScaleChanged()
{
    updateChildrenTransform();
}

void Entity3D::onRotationChanged()
{
    updateChildrenTransform();
}

void Entity3D::lookAt(const Vector3f & targetPosition)
{
    Vector3f v = targetPosition - getGlobalPosition();
    Vector3f euler = getSphericalAngles(v);
    Quaternion q;
    q.setFromEuler(euler.x(), euler.y(), euler.z());
    setRotation(q);
}

void Entity3D::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity::serializeState(o, context);
    sn::serialize(o["position"], m_position);
    sn::serialize(o["rotation"], m_rotation);
    sn::serialize(o["scale"], m_scale);
}

void Entity3D::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity::unserializeState(o, context);
    sn::unserialize(o["position"], m_position);
    sn::unserialize(o["rotation"], m_rotation);
    sn::unserialize(o["scale"], m_scale);
}

} // namespace sn

