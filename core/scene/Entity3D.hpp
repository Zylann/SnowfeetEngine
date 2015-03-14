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

    Vector3f getPosition();
    void setPosition(const Vector3f & newPos);

    Quaternion getRotation();
    void setRotation(const Quaternion & newRotation);

    Vector3f getScale();
    void setScale(const Vector3f & newScale);

    //--------------------------------
    // Global
    //--------------------------------

    Vector3f getGlobalPosition();
    void setGlobalPosition(const Vector3f & newPos);

    Quaternion getGlobalRotation();
    void setGlobalRotation(const Quaternion & newRotation);

    Vector3f getGlobalScale();
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

    //--------------------------------
    // Serialization
    //--------------------------------

    virtual void serializeState(JsonBox::Value & o) override;
    virtual void unserializeState(JsonBox::Value & o) override;

protected:
    virtual void onPositionChanged();
    virtual void onScaleChanged();
    virtual void onRotationChanged();

private:
    void updateChildrenTransform();

private:
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

