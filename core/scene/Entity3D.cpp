#include "Entity3D.hpp"

namespace sn
{

Entity3D::Entity3D() : Entity(), 
    m_localMatrixNeedUpdate(true), 
    m_globalMatrixNeedUpdate(true)
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
    // TODO
}

const Matrix4 & Entity3D::getLocalMatrix() const
{
    if (m_localMatrixNeedUpdate)
    {
        // TODO

    }

}

const Matrix4 & Entity3D::getGlobalMatrix() const
{
    if (m_globalMatrixNeedUpdate)
    {
        // TODO

    }

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

void Entity3D::serializeState(JsonBox::Value & o)
{
    Entity::serializeState(o);
    sn::serialize(o["position"], m_position);
    sn::serialize(o["rotation"], m_rotation]);
    sn::serialize(o["scale"], m_scale);
}

void Entity3D::unserializeState(JsonBox::Value & o)
{
    Entity::unserializeState(o);
    sn::unserialize(o["position"], m_position);
    sn::unserialize(o["rotation"], m_rotation]);
    sn::unserialize(o["scale"], m_scale);
}

} // namespace sn

