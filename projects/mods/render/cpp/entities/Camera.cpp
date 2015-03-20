#include "Camera.hpp"

namespace sn {
namespace render {

const std::string Camera::TAG = "Camera";

//------------------------------------------------------------------------------
void Camera::setOrtho(bool ortho)
{
    m_isOrtho = ortho;
    m_projectionMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
void Camera::setOrthoSize(Vector2f orthoSize)
{
    m_orthoSize = orthoSize;
    m_projectionMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
void Camera::setFov(f32 newFov)
{
    m_fov = newFov;
    m_projectionMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
void Camera::setNear(f32 newNear)
{
    m_near = newNear;
    m_projectionMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
void Camera::setFar(f32 newFar)
{
    m_far = newFar;
    m_projectionMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
void Camera::setAspectRatio(f32 newAspectRatio)
{
    m_aspectRatio = newAspectRatio;
    m_projectionMatrixNeedUpdate = true;
}

//------------------------------------------------------------------------------
const Matrix4 & Camera::getProjectionMatrix() const
{
    if (m_projectionMatrixNeedUpdate)
    {
        if (m_isOrtho)
        {
            m_projectionMatrix.loadPerspectiveProjection(
                math::degToRad(m_fov), 
                m_aspectRatio, 
                m_near, m_far
            );
        }
        else
        {
            m_projectionMatrix.loadOrtho2DProjection(
                -m_orthoSize.x(), 
                m_orthoSize.x(), 
                m_orthoSize.y()*m_aspectRatio, 
                m_orthoSize.y()*m_aspectRatio
            );
        }
        m_projectionMatrixNeedUpdate = false;
    }

    return m_projectionMatrix;
}

//------------------------------------------------------------------------------
void Camera::serializeState(JsonBox::Value & o)
{
    Entity3D::serializeState(o);

    sn::serialize(o["near"], m_near);
    sn::serialize(o["far"], m_far);
    sn::serialize(o["isOrtho"], m_isOrtho);
    sn::serialize(o["aspectRatio"], m_aspectRatio);
    sn::serialize(o["fov"], m_fov);
    sn::serialize(o["orthoSize"], m_orthoSize);
    sn::serialize(o["drawOrder"], m_drawOrder);
}

//------------------------------------------------------------------------------
void Camera::unserializeState(JsonBox::Value & o)
{
    Entity3D::unserializeState(o);

    sn::unserialize(o["near"], m_near);
    sn::unserialize(o["far"], m_far);
    sn::unserialize(o["isOrtho"], m_isOrtho);
    sn::unserialize(o["aspectRatio"], m_aspectRatio);
    sn::unserialize(o["fov"], m_fov);
    sn::unserialize(o["orthoSize"], m_orthoSize);
    sn::unserialize(o["drawOrder"], m_drawOrder);

	m_projectionMatrixNeedUpdate = true;
}


} // namespace render
} // namespace sn

