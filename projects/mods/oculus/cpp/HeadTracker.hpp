#ifndef __HEADER_OCULUS_HEAD_TRACKER__
#define __HEADER_OCULUS_HEAD_TRACKER__

#include <OVR.h>
#include <OVR_CAPI.h>

#include <core/scene/VRHeadset.h>
#include <core/asset/base/Mesh.h>
#include <core/asset/base/Material.h>
#include <core/util/SharedRef.h>

namespace sn {
namespace oculus {

// TODO Rename RiftController
class HeadTracker : public sn::VRHeadset
{
public:
    SN_ENTITY(sn::oculus::HeadTracker, sn::VRHeadset)

    HeadTracker();

    void onReady() override;

    void onRenderEye(
        Entity * sender, 
        VRHeadset::EyeIndex abstractEyeIndex,
        Material * effectMaterial, 
        Vector2u sourceSize, 
        IntRect targetViewport
    ) override;

    Vector2u getPreferredFramebufferSize(EyeIndex eye) override;

    // No special serialization needed at the moment

protected:
    ~HeadTracker();

    void onUpdate() override;

private:
    void makeDistortionMesh(Mesh & out_mesh, u32 agnosticEyeType);

private:
    ovrHmd m_ovrHmd;
    ovrFrameTiming m_ovrFrameTiming;
    ovrEyeRenderDesc m_ovrEyeDesc[2];
    f32 m_lastYaw;
    bool m_isFirstUpdate;

};

} // namespace oculus
} // namespace sn

#endif // __HEADER_OCULUS_HEAD_TRACKER__

