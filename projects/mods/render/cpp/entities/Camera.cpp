#include <core/system/gui/SystemGUI.h>
#include <core/asset/AssetDatabase.h>

#include "Drawable.hpp"
#include "Camera.hpp"

namespace sn {
namespace render {

const std::string Camera::TAG = "Camera";

//------------------------------------------------------------------------------
void serialize(JsonBox::Value & o, ScaleMode m)
{
    switch (m)
    {
    case SNR_SCALEMODE_ADAPTED: o = "adapted"; break;
    case SNR_SCALEMODE_PIXELPERFECT: o = "pixelperfect"; break;
    default: o = "none"; break;
    }
}

//------------------------------------------------------------------------------
void unserialize(JsonBox::Value & o, ScaleMode & m)
{
    std::string s = o.getString();
    if (s == "adapted")
        m = SNR_SCALEMODE_ADAPTED;
    else if (s == "pixelperfect")
        m = SNR_SCALEMODE_PIXELPERFECT;
    else
        m = SNR_SCALEMODE_NONE;
}

//------------------------------------------------------------------------------
Camera::Camera(): Entity3D(),
    m_isOrtho(false),
    m_fov(70.f),
    m_near(0.1f),
    m_far(100.f),
    m_drawOrder(0),
    m_aspectRatio(1),
    m_scaleMode(SNR_SCALEMODE_ADAPTED),
    m_clearBits(SNR_CLEAR_COLOR | SNR_CLEAR_DEPTH),
    m_visibilityTag(Drawable::TAG),
    m_viewport(0,0,1,1),
    m_projectionMatrixNeedUpdate(true),
    m_targetWindowID(0)
{
    // TODO Have a zeroMemory template function helper
    memset(m_effectBuffers, 0, sizeof(RenderTexture*) * EFFECT_BUFFERS_COUNT);
}

//------------------------------------------------------------------------------
Camera::~Camera()
{
    for (u32 i = 0; i < EFFECT_BUFFERS_COUNT; ++i)
    {
        if (m_effectBuffers[i])
            m_effectBuffers[i]->release();
    }
}

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
void Camera::setClearColor(const Color & clearColor)
{
    m_clearColor = clearColor;
}

//------------------------------------------------------------------------------
void Camera::setScaleMode(ScaleMode mode)
{
    m_scaleMode = mode;
}

//------------------------------------------------------------------------------
void Camera::setClearBits(ClearMask mask)
{
    m_clearBits = mask;
}

//------------------------------------------------------------------------------
void Camera::setVisibilityTag(const std::string & tag)
{
    m_visibilityTag = tag;
}

//------------------------------------------------------------------------------
void Camera::setRenderTexture(RenderTexture * rt)
{
    r_renderTexture.set(rt);
    onTargetResized();
    //updateAspectRatio();
}

//------------------------------------------------------------------------------
void Camera::setTargetWindowByID(u32 windowID)
{
    m_targetWindowID = windowID;
}

//------------------------------------------------------------------------------
u32 Camera::getTargetWindowID() const
{
    return m_targetWindowID;
}

//------------------------------------------------------------------------------
Window * Camera::getTargetWindow() const
{
    if (r_renderTexture.isNull())
    {
        Window * win = SystemGUI::get().getWindowByID(m_targetWindowID);
        return win;
    }
    else
    {
        return nullptr;
    }
}

//------------------------------------------------------------------------------
void Camera::setViewport(FloatRect normalizedRect)
{
    m_viewport = normalizedRect;
    //updateAspectRatio();
    onTargetResized();
}

//------------------------------------------------------------------------------
void Camera::updateAspectRatio()
{
    if (m_scaleMode == SNR_SCALEMODE_ADAPTED)
    {
        IntRect viewport = getPixelViewport();
        if (viewport.height() != 0)
        {
            setAspectRatio(static_cast<f32>(viewport.width()) / static_cast<f32>(viewport.height()));
        }
    }
}

//------------------------------------------------------------------------------
void Camera::updateOrthographicSize()
{
    if (m_scaleMode == SNR_SCALEMODE_PIXELPERFECT)
    {
        // Match pixel size
        IntRect viewport = getPixelViewport();
        setOrthoSize(viewport.size());
    }
    else if (m_scaleMode == SNR_SCALEMODE_ADAPTED && !math::isZero(m_orthoSize.y()))
    {
        // Adapt width of the view
        IntRect viewport = getPixelViewport();
        //f32 oldRatio = m_orthoSize.x() / m_orthoSize.y();
        f32 newRatio = static_cast<f32>(viewport.width()) / static_cast<f32>(viewport.height());
        m_orthoSize.x() = m_orthoSize.y() * newRatio;
        m_projectionMatrixNeedUpdate = true;
    }
}

//------------------------------------------------------------------------------
void Camera::onTargetResized()
{
    if (m_isOrtho)
    {
        updateOrthographicSize();
    }
    else
    {
        updateAspectRatio();
    }
}

//------------------------------------------------------------------------------
IntRect Camera::getPixelViewport() const
{
    // Get target size
    Vector2u targetSize;
    if (!r_renderTexture.isNull())
    {
        targetSize = r_renderTexture.get()->getSize();
    }
    else
    {
        Window * win = getTargetWindow();
        if (win)
        {
            targetSize = win->getClientSize();
        }
    }

    // Return pixel rect
    return IntRect(
        static_cast<s32>( m_viewport.x() * static_cast<f32>(targetSize.x()) ),
        static_cast<s32>( m_viewport.y() * static_cast<f32>(targetSize.y()) ),
        static_cast<s32>( m_viewport.width() * static_cast<f32>(targetSize.x()) ),
        static_cast<s32>( m_viewport.height() * static_cast<f32>(targetSize.y()) )
    );
}

//------------------------------------------------------------------------------
const Matrix4 & Camera::getProjectionMatrix() const
{
    if (m_projectionMatrixNeedUpdate)
    {
        if (m_isOrtho)
        {
            Vector2f orthoSize = m_orthoSize * 0.5f;
            m_projectionMatrix.loadOrtho2DProjection(
                -orthoSize.x(),
                orthoSize.y(),
                orthoSize.x(),
                -orthoSize.y(),
                m_near, 
                m_far
            );
        }
        else
        {
            m_projectionMatrix.loadPerspectiveProjection(
                math::degToRad(m_fov),
                m_aspectRatio,
                m_near, m_far
            );
        }
        m_projectionMatrixNeedUpdate = false;
    }

    return m_projectionMatrix;
}

//------------------------------------------------------------------------------
Matrix4 Camera::getViewMatrix() const
{
    Vector3f up = getUpVector();
    Vector3f forward = getForwardVector();
    Vector3f cameraPosition = getGlobalPosition();

    Matrix4 viewMatrix;
    viewMatrix.loadLookAt(cameraPosition, cameraPosition + forward, up);

    return viewMatrix;
}

//------------------------------------------------------------------------------
void Camera::onReady()
{
    addTag(TAG);
    updateAspectRatio();
}

//------------------------------------------------------------------------------
void Camera::addEffect(Material * effectMaterial, Mesh * mesh)
{
    updateEffectBuffers();
    m_effects.push_back(Effect());
    Effect & effect = m_effects.back();
    effect.material.set(effectMaterial);
    effect.mesh.set(mesh);
}

//------------------------------------------------------------------------------
void Camera::updateEffectBuffers(const Vector2u * overrideResolution)
{
    // Get which resolution to use
    Vector2u targetSize;
    if (overrideResolution)
    {
        targetSize = *overrideResolution;
    }
    else
    {
        IntRect viewport = getPixelViewport();
        targetSize.x() = viewport.width();
        targetSize.y() = viewport.height();
    }

    // Check zero-size
    if (targetSize.x() == 0 || targetSize.y() == 0)
    {
        SN_WARNING("Camera::updateEffectBuffers: targetSize is " << sn::toString(targetSize));
        return;
    }

    // Delete buffers if the target size changed
    if (m_effectBuffers[0] != nullptr)
    {
        RenderTexture & rt = *m_effectBuffers[0];
        if (rt.getSize() != targetSize)
        {
            for (u32 i = 0; i < EFFECT_BUFFERS_COUNT; ++i)
            {
                m_effectBuffers[i]->release();
                m_effectBuffers[i] = nullptr;
            }
        }
    }

    // Create buffers if they are not there
    if (m_effectBuffers[0] == nullptr)
    {
        for (u32 i = 0; i < EFFECT_BUFFERS_COUNT; ++i)
        {
            RenderTexture * rt = new RenderTexture();
            rt->create(targetSize);
            m_effectBuffers[i] = rt;
        }
    }
}

//------------------------------------------------------------------------------
void Camera::serializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity3D::serializeState(o, context);

    sn::serialize(o["near"], m_near);
    sn::serialize(o["far"], m_far);
    sn::serialize(o["isOrtho"], m_isOrtho);
    render::serialize(o["scaleMode"], m_scaleMode);
    sn::serialize(o["fov"], m_fov);
    sn::serialize(o["orthoSize"], m_orthoSize);
    sn::serialize(o["drawOrder"], m_drawOrder);
    render::serialize(o["clearBits"], m_clearBits);
    sn::serialize(o["clearColor"], m_clearColor);
    sn::serialize(o["viewport"], m_viewport);
    sn::serialize(o["targetWindow"], m_targetWindowID);
    sn::serialize(o["visibilityTag"], m_visibilityTag);

    if (!r_renderTexture.isNull())
    {
        ;// TODO serialize target
    }
}

//------------------------------------------------------------------------------
void Camera::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity3D::unserializeState(o, context);

    sn::unserialize(o["near"], m_near, 0.1f);
    sn::unserialize(o["far"], m_far, 100.f);
    sn::unserialize(o["isOrtho"], m_isOrtho, false);
    render::unserialize(o["scaleMode"], m_scaleMode);
    sn::unserialize(o["fov"], m_fov, 70.f);
    sn::unserialize(o["orthoSize"], m_orthoSize, Vector2f(16,9));
    sn::unserialize(o["drawOrder"], m_drawOrder);
    render::unserialize(o["clearBits"], m_clearBits);
    sn::unserialize(o["clearColor"], m_clearColor, Color());
    sn::unserialize<u32>(o["targetWindow"], m_targetWindowID, 0);
    sn::unserialize(o["visibilityTag"], m_visibilityTag, m_visibilityTag);

    if (!o["viewport"].isNull())
    {
        FloatRect viewport;
        sn::unserialize(o["viewport"], viewport, FloatRect(0,0,1,1));
        setViewport(viewport);
    }

    auto & effects = o["effects"];
    if (effects.isArray())
    {
        u32 count = effects.getArray().size();
        for (u32 i = 0; i < count; ++i)
        {
            if (effects[i].isString())
            {
                Material * mat = getAssetBySerializedLocation<Material>(effects[i].getString(), context.getModule(), this);
                if (mat != nullptr)
                {
                    addEffect(mat);
                }
                else
                {
                    SN_ERROR("Effect material not found: '" << effects[i].getString() << "'");
                }
            }
            else if (effects[i].isObject())
            {
                JsonBox::Value & o = effects[i];
                Material * mat = getAssetBySerializedLocation<Material>(o["material"].getString(), context.getModule(), this);
                Mesh * mesh = getAssetBySerializedLocation<Mesh>(o["mesh"].getString(), context.getModule(), this);
                if (mat)
                {
                    addEffect(mat, mesh);
                }
            }
            else
            {
                SN_ERROR("Effect material is not a string");
                break;
            }
        }
    }

    setRenderTexture(getAssetBySerializedLocation<RenderTexture>(o["renderTexture"].getString(), context.getModule(), this));

	m_projectionMatrixNeedUpdate = true;
}

} // namespace render
} // namespace sn

