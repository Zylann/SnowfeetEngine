#ifndef __HEADER_SN_RENDER_CAMERA__
#define __HEADER_SN_RENDER_CAMERA__

#include <core/scene/Entity3D.hpp>
#include <core/math/Vector2.hpp>

namespace sn {
namespace render {

enum ClearMode
{
    SNR_CLEAR_NONE,
    SNR_CLEAR_COLOR
    //SNR_CLEAR_SKYBOX
};

void serialize(JsonBox::Value & o, ClearMode m);
void unserialize(JsonBox::Value & o, ClearMode & m);

enum ScaleMode
{
    SNR_SCALEMODE_NONE,
    SNR_SCALEMODE_ADAPTED
};

void serialize(JsonBox::Value & o, ScaleMode m);
void unserialize(JsonBox::Value & o, ScaleMode & m);

class Camera : public Entity3D
{
public:

    SN_ENTITY(sn::render::Camera, sn::Entity3D)

    static const std::string TAG;

    Camera():
        Entity3D(),
        m_isOrtho(false),
        m_fov(70.f),
        m_near(0.1f),
        m_far(100.f),
        m_drawOrder(0),
        m_aspectRatio(1),
        m_scaleMode(SNR_SCALEMODE_ADAPTED),
        m_clearMode(SNR_CLEAR_NONE),
        m_projectionMatrixNeedUpdate(true)
    {}

    void setOrtho(bool ortho);
    inline bool isOrtho() const { return m_isOrtho; }

    void setOrthoSize(Vector2f orthoSize);
    inline const Vector2f & getOrthoSize() const { return m_orthoSize; }

    inline f32 getFov() const { return m_fov; }
    inline f32 getNear() const { return m_near; }
    inline f32 getFar() const { return m_far; }
    inline f32 getAspectRatio() const { return m_aspectRatio; }
    inline const Color & getClearColor() const { return m_clearColor; }
    inline ClearMode getClearMode() const { return m_clearMode; }
    inline ScaleMode getScaleMode() const { return m_scaleMode; }

    void setFov(f32 newFov);
    void setNear(f32 newNear);
    void setFar(f32 newFar);
    void setAspectRatio(f32 newAspectRatio);
    void setClearColor(const Color & clearColor);
    void setClearMode(ClearMode mode);
    void setScaleMode(ScaleMode mode);

    const Matrix4 & getProjectionMatrix() const;

    inline s32 getDrawOrder() const { return m_drawOrder; }
    inline void setDrawOrder(s32 order) { m_drawOrder = order; }

    //void setVisibilityMask(u32 mask);

    //-----------------------------------------
    // Event handlers
    //-----------------------------------------

    void onReady() override;

    void onTargetResized(u32 width, u32 height);

    //-----------------------------------------
    // Serialization
    //-----------------------------------------

    void serializeState(JsonBox::Value & o, const SerializationContext & context) override;
    void unserializeState(JsonBox::Value & o, const SerializationContext & context) override;

private:
    //std::bitset<32> m_cullingMask;
    bool m_isOrtho;
    Vector2f m_orthoSize;
    f32 m_fov;
    f32 m_near;
    f32 m_far;
    s32 m_drawOrder;

    f32 m_aspectRatio;
    ScaleMode m_scaleMode;

    Color m_clearColor;
    ClearMode m_clearMode;

    mutable bool m_projectionMatrixNeedUpdate;
    mutable Matrix4 m_projectionMatrix;

    //Target m_target;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_CAMERA__


