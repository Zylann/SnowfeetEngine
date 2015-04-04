#ifndef __HEADER_SN_ENTITY3D__
#define __HEADER_SN_ENTITY3D__

#include <core/scene/Entity.hpp>

namespace sn
{

class SN_API Entity3D : public Entity
{
public:
    SN_ENTITY(sn::Entity3D, sn::Entity)

    Entity3D();

    //--------------------------------
    // Local
    //--------------------------------

    Vector3f getPosition() const;
    void setPosition(const Vector3f & newPos);

    Quaternion getRotation() const;
    void setRotation(const Quaternion & newRotation);

    Vector3f getScale() const;
    void setScale(const Vector3f & newScale);

    //--------------------------------
    // Global
    //--------------------------------

    Vector3f getGlobalPosition() const;
    void setGlobalPosition(const Vector3f & newPos);

    Quaternion getGlobalRotation() const;
    void setGlobalRotation(const Quaternion & newRotation);

    Vector3f getGlobalScale() const;
    void setGlobalScale(const Vector3f & newScale);

    //--------------------------------
    // Matrices
    //--------------------------------

    const Matrix4 & getLocalMatrix() const;
    const Matrix4 & getGlobalMatrix() const;

    //--------------------------------
    // Helpers
    //--------------------------------

    void lookAt(const Vector3f & targetPosition);

    Vector3f getRightVector() const;
    Vector3f getUpVector() const;
    Vector3f getForwardVector() const;

    //--------------------------------
    // Serialization
    //--------------------------------

    virtual void serializeState(JsonBox::Value & o, const SerializationContext & context) override;
    virtual void unserializeState(JsonBox::Value & o, const SerializationContext & context) override;

protected:
    virtual void onPositionChanged();
    virtual void onScaleChanged();
    virtual void onRotationChanged();

private:
    void updateChildrenTransform();

private:
    // Local
    Vector3f m_position;
    Vector3f m_scale;
    Quaternion m_rotation;

    mutable Matrix4 m_localMatrix;
    mutable Matrix4 m_globalMatrix;

    mutable bool m_localMatrixNeedUpdate;
    mutable bool m_globalMatrixNeedUpdate;

};

} // namespace sn

#endif // __HEADER_SN_ENTITY3D__

