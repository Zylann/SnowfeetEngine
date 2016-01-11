#include <core/sml/variant_serialize.h>
#include <core/app/Application.h>
#include "CameraRenderStep.h"

namespace sn
{

SN_OBJECT_IMPL(CameraRenderStep)

CameraRenderStep::CameraRenderStep() : RenderStep()
{
    m_tag = Camera::TAG;
}

void CameraRenderStep::onDraw(VideoDriver & driver)
{
    RenderManager * manager = getManager();
    if (manager == nullptr)
        return;
    manager->renderAllTaggedCameras(driver, m_tag);
}

void CameraRenderStep::serializeState(Variant & o, const SerializationContext & context)
{
    RenderStep::serializeState(o, context);
    sn::serialize(o["tag"], m_tag);
}

void CameraRenderStep::unserializeState(const Variant & o, const SerializationContext & context)
{
    RenderStep::unserializeState(o, context);
    sn::unserialize(o["tag"], m_tag);
}

} // namespace sn

