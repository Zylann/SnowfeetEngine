#ifndef __HEADER_SN_RENDER_CAMERA__
#define __HEADER_SN_RENDER_CAMERA__

#include <core/scene/Entity3D.hpp>
#include <core/math/Vector2.hpp>
#include <core/util/WeakRef.hpp>
#include <core/asset/base/Mesh.hpp>

#include "../ClearBits.hpp"
#include "../Material.hpp"
#include "../RenderTexture.hpp"

namespace sn {
namespace render {

//------------------------------------------------------------------------------
enum ScaleMode
{
    SNR_SCALEMODE_NONE,
    SNR_SCALEMODE_ADAPTED
};

void serialize(JsonBox::Value & o, ScaleMode m);
void unserialize(JsonBox::Value & o, ScaleMode & m);

//------------------------------------------------------------------------------
/// \brief Point of view on the scene, from which things can be rendered.
class Camera : public Entity3D
{
public:

    SN_ENTITY(sn::render::Camera, sn::Entity3D)

    static const std::string TAG;

    struct Effect
    {
        /// \brief Material used to post-process the image
        SharedRef<Material> material;

        /// \brief Optional mesh used to distort the image
        /// \note Initially added to support Oculus Rift mesh-based distortion
        SharedRef<Mesh> mesh;
    };

    Camera():
        Entity3D(),
        m_isOrtho(false),
        m_fov(70.f),
        m_near(0.1f),
        m_far(100.f),
        m_drawOrder(0),
        m_aspectRatio(1),
        m_scaleMode(SNR_SCALEMODE_ADAPTED),
        m_clearBits(SNR_CLEAR_COLOR | SNR_CLEAR_DEPTH),
        m_viewport(0,0,1,1),
        m_projectionMatrixNeedUpdate(true),
        m_targetWindowID(0)
    {
        // TODO Have a zeroMemory template function helper
        memset(m_effectBuffers, 0, sizeof(RenderTexture*) * EFFECT_BUFFERS_COUNT);
    }

    ~Camera();

    //-----------------------------------------
    // Camera interface
    //-----------------------------------------

    void setOrtho(bool ortho);
    inline bool isOrtho() const { return m_isOrtho; }

    void setOrthoSize(Vector2f orthoSize);
    inline const Vector2f & getOrthoSize() const { return m_orthoSize; }

    inline f32 getFov() const { return m_fov; }
    inline f32 getNear() const { return m_near; }
    inline f32 getFar() const { return m_far; }
    inline f32 getAspectRatio() const { return m_aspectRatio; }
    inline const Color & getClearColor() const { return m_clearColor; }
    inline ClearMask getClearBits() const { return m_clearBits; }
    inline ScaleMode getScaleMode() const { return m_scaleMode; }

    /// \brief Gets the viewport's coordinates in normalized space (-1 to 1 horizontally and vertically).
    inline const FloatRect & getViewport() const { return m_viewport; }

    /// \brief Gets the viewport's coordinates in pixels.
    /// It corresponds to the actual rectangle where this camera will draw on the target (screen or framebuffer).
    /// \warning It can be zero-sized if the target window is not set or minimized by the system.
    IntRect getPixelViewport() const;

    void setFov(f32 newFov);
    void setNear(f32 newNear);
    void setFar(f32 newFar);
    void setAspectRatio(f32 newAspectRatio);
    void setClearColor(const Color & clearColor);
    void setClearBits(ClearMask mask);
    void setScaleMode(ScaleMode mode);
    void setViewport(FloatRect normalizedRect);

    const Matrix4 & getProjectionMatrix() const;
    Matrix4 getViewMatrix() const;

    inline s32 getDrawOrder() const { return m_drawOrder; }
    inline void setDrawOrder(s32 order) { m_drawOrder = order; }

    void setRenderTexture(RenderTexture * rt);
    RenderTexture * getRenderTarget() const { return r_renderTexture.get(); }

    void setTargetWindowByID(u32 windowID);
    u32 getTargetWindowID() const;
    Window * getTargetWindow() const;

    //void setVisibilityMask(u32 mask);

    // TODO This is a temporary API, it might change in the future!
    void addEffect(Material * effectMaterial, Mesh * mesh=nullptr);
    u32 getEffectCount() const { return m_effects.size(); }
    RenderTexture * getEffectBuffer(u32 i) const { return m_effectBuffers[i]; }
    const Effect & getEffect(u32 i) const { return m_effects[i]; }

    //-----------------------------------------
    // Event handlers
    //-----------------------------------------

    void onReady() override;

    //void onTargetResized(u32 width, u32 height);
    void updateAspectRatio();

    //-----------------------------------------
    // Serialization
    //-----------------------------------------

    void serializeState(JsonBox::Value & o, const SerializationContext & context) override;
    void unserializeState(JsonBox::Value & o, const SerializationContext & context) override;

    // TODO Don't leave this public, it should be automated (for target textures resizing)
    void updateEffectBuffers(const Vector2u * overrideResolution = nullptr);
private:

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
    ClearMask m_clearBits;

    FloatRect m_viewport;

    mutable bool m_projectionMatrixNeedUpdate;
    mutable Matrix4 m_projectionMatrix;

    WeakRef<RenderTexture> r_renderTexture;
    u32 m_targetWindowID;

    static const u32 EFFECT_BUFFERS_COUNT = 2;

    std::vector<Effect> m_effects;
    RenderTexture * m_effectBuffers[EFFECT_BUFFERS_COUNT];

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_CAMERA__


