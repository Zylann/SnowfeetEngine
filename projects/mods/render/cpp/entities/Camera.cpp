#include "Camera.hpp"

namespace sn {
namespace render {

void Camera::lookAt(Vector3f position)
{
    // TODO
}

void Camera::setOrtho(bool ortho)
{
    m_isOrtho = ortho;
    // TODO
}

void Camera::setOrthoSize(Vector2f orthoSize)
{
    m_orthoSize = orthoSize;
    // TODO
}

} // namespace render
} // namespace sn

