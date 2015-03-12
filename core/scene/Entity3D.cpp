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
    // TODO
}

void Entity3D::setGlobalPosition(const Vector3f & newPos)
{
    // TODO
}

Quaternion Entity3D::getGlobalRotation()
{
    // TODO
}

void Entity3D::setGlobalRotation(const Quaternion & newRotation)
{
    // TODO
}

Vector3f Entity3D::getGlobalScale()
{
    // TODO
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

