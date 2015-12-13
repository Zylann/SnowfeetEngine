#ifndef __HEADER_SN_RENDER_CAMERA__
#define __HEADER_SN_RENDER_CAMERA__

#include <core/scene/Entity3D.h>
#include <core/math/Vector2.h>
#include <core/math/Rect.h>
#include <core/util/WeakRef.h>
#include <core/system/Window.h>

#include <modules/render/Mesh.h>

#include "../ClearBits.h"
#include "../Material.h"
#include "../RenderTexture.h"

namespace sn
{

//------------------------------------------------------------------------------
enum ScaleMode
{
    /// \brief The view will be stretched from a 1:1 aspect ratio
    SNR_SCALEMODE_NONE,
    /// \brief The aspect ratio will be adjusted to match the target screen
    SNR_SCALEMODE_ADAPTED,
    /// \brief in orthographic mode, the size of the view will match the pixel size of the screen
    SNR_SCALEMODE_PIXELPERFECT
};

void serialize(sn::Variant & o, ScaleMode m);
void unserialize(const sn::Variant & o, ScaleMode & m);

//------------------------------------------------------------------------------
/// \brief Point of view on the scene, from which things can be rendered.
class Camera : public Entity3D
{
public:

    SN_OBJECT(sn::Camera, sn::Entity3D)

    static const std::string TAG;

    struct Effect
    {
        /// \brief Material used to post-process the image
        SharedRef<Material> material;

        /// \brief Optional mesh used to distort the image
        /// \note Initially added to support Oculus Rift mesh-based distortion
        SharedRef<Mesh> mesh;
    };

    Camera();

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
    inline const std::string & getVisibilityTag() const { return m_visibilityTag; }

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
    void setVisibilityTag(const std::string & tag);

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

    void onTargetResized();

    //-----------------------------------------
    // Serialization
    //-----------------------------------------

    void serializeState(sn::Variant & o, const SerializationContext & context) override;
    void unserializeState(const sn::Variant & o, const SerializationContext & context) override;

    // TODO Don't leave this public, it should be automated (for target textures resizing)
    void updateEffectBuffers(const Vector2u * overrideResolution = nullptr);
private:
    ~Camera();

    void updateAspectRatio();
    void updateOrthographicSize();

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

    std::string m_visibilityTag;

    FloatRect m_viewport;

    mutable bool m_projectionMatrixNeedUpdate;
    mutable Matrix4 m_projectionMatrix;

    WeakRef<RenderTexture> r_renderTexture;
    u32 m_targetWindowID;

    static const u32 EFFECT_BUFFERS_COUNT = 2;

    std::vector<Effect> m_effects;
    RenderTexture * m_effectBuffers[EFFECT_BUFFERS_COUNT];

};

} // namespace sn

#endif // __HEADER_SN_RENDER_CAMERA__


