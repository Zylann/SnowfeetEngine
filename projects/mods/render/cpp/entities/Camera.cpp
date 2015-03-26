#include <core/system/gui/SystemGUI.hpp>
#include <core/asset/AssetDatabase.hpp>

#include "Camera.hpp"

namespace sn {
namespace render {

const std::string Camera::TAG = "Camera";

//------------------------------------------------------------------------------
void serialize(JsonBox::Value & o, ClearMode m)
{
    switch (m)
    {
    case SNR_CLEAR_COLOR: o = "color"; break;
    default: o = "none"; break;
    }
}

//------------------------------------------------------------------------------
void unserialize(JsonBox::Value & o, ClearMode & m)
{
    std::string s = o.getString();
    if (s == "color")
        m = SNR_CLEAR_COLOR;
    else
        m = SNR_CLEAR_NONE;
}

//------------------------------------------------------------------------------
void serialize(JsonBox::Value & o, ScaleMode m)
{
    switch (m)
    {
    case SNR_SCALEMODE_ADAPTED: o = "adapted"; break;
    default: o = "none"; break;
    }
}

//------------------------------------------------------------------------------
void unserialize(JsonBox::Value & o, ScaleMode & m)
{
    std::string s = o.getString();
    if (s == "adapted")
        m = SNR_SCALEMODE_ADAPTED;
    else
        m = SNR_SCALEMODE_NONE;
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
void Camera::setClearMode(ClearMode mode)
{
    m_clearMode = mode;
}

//------------------------------------------------------------------------------
void Camera::setRenderTexture(RenderTexture * rt)
{
    r_renderTexture.set(rt);
    //if (rt)
    //{
    //    Vector2u size = rt->getSize();
    //    onTargetResized(size.x(), size.y());
    //}
    updateAspectRatio();
}

//------------------------------------------------------------------------------
void Camera::setViewport(FloatRect normalizedRect)
{
    m_viewport = normalizedRect;
    updateAspectRatio();
    //onTargetResized(rect.width(), rect.height());
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
        // TODO Fix this for multi windows!
        Window * win = SystemGUI::get().getWindowByID(0);
        if (win)
            targetSize = win->getClientSize();
    }

    // Return pixel rect
    return IntRect(
        m_viewport.x() * targetSize.x(),
        m_viewport.y() * targetSize.y(),
        m_viewport.width() * targetSize.x(),
        m_viewport.height() * targetSize.y()
    );
}

//------------------------------------------------------------------------------
const Matrix4 & Camera::getProjectionMatrix() const
{
    if (m_projectionMatrixNeedUpdate)
    {
        if (m_isOrtho)
        {
            m_projectionMatrix.loadOrtho2DProjection(
                -m_orthoSize.x(),
                m_orthoSize.y()*m_aspectRatio,
                m_orthoSize.x(),
                -m_orthoSize.y()*m_aspectRatio
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
void Camera::onReady()
{
    addTag(TAG);
    updateAspectRatio();
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
    render::serialize(o["clearMode"], m_clearMode);
    sn::serialize(o["clearColor"], m_clearColor);
    sn::serialize(o["viewport"], m_viewport);

    if (!r_renderTexture.isNull())
    {
        ;// TODO serialize target
    }
}

//------------------------------------------------------------------------------
void Camera::unserializeState(JsonBox::Value & o, const SerializationContext & context)
{
    Entity3D::unserializeState(o, context);

    sn::unserialize(o["near"], m_near);
    sn::unserialize(o["far"], m_far);
    sn::unserialize(o["isOrtho"], m_isOrtho);
    render::unserialize(o["scaleMode"], m_scaleMode);
    sn::unserialize(o["fov"], m_fov);
    sn::unserialize(o["orthoSize"], m_orthoSize);
    sn::unserialize(o["drawOrder"], m_drawOrder);
    render::unserialize(o["clearMode"], m_clearMode);
    sn::unserialize(o["clearColor"], m_clearColor);

    if (!o["viewport"].isNull())
    {
        FloatRect viewport;
        sn::unserialize(o["viewport"], viewport);
        setViewport(viewport);
    }

    setRenderTexture(getAssetBySerializedLocation<RenderTexture>(o["renderTexture"].getString(), context.getModule(), this));

	m_projectionMatrixNeedUpdate = true;
}

} // namespace render
} // namespace sn

