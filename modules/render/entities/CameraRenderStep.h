#ifndef __HEADER_SN_CAMERA_RENDER_STEP__
#define __HEADER_SN_CAMERA_RENDER_STEP__

#include <modules/render/entities/Camera.h>
#include <modules/render/entities/RenderStep.h>

namespace sn {
namespace render {

class CameraRenderStep : public RenderStep
{
public:
    SN_ENTITY(sn::render::CameraRenderStep, sn::render::RenderStep)

    CameraRenderStep();

    void onDraw(VideoDriver & driver) override;

    void serializeState(Variant & o, const SerializationContext & context) override;
    void unserializeState(const Variant & o, const SerializationContext & context) override;

private:
    std::string m_tag;
};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_CAMERA_RENDER_STEP__

