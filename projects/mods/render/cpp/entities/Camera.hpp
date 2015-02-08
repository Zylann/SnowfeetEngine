#ifndef __HEADER_SN_RENDER_CAMERA__
#define __HEADER_SN_RENDER_CAMERA__

#include <core/scene/Entity.hpp>
#include <core/math/Vector2.hpp>

namespace sn {
namespace render {

class Camera : public Entity
{
public:

    SN_ENTITY(sn::render::Camera, sn::Entity)

    Camera():
        Entity(),
        m_isOrtho(false)
    {}

    void lookAt(Vector3f position);

    void setOrtho(bool ortho);
    inline bool isOrtho() const { return m_isOrtho; }

    void setOrthoSize(Vector2f orthoSize);
    inline const Vector2f & getOrthoSize() const { return m_orthoSize; }

    const Matrix4 & getProjectionMatrix() const { return m_projectionMatrix; }

    //void setVisibilityMask(u32 mask);

private:

    //std::bitset<32> m_cullingMask;
    bool m_isOrtho;
    Vector2f m_orthoSize;
    Matrix4 m_projectionMatrix;
    //Target m_target;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_CAMERA__


