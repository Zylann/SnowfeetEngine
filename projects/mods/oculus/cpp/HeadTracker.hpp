#ifndef __HEADER_OCULUS_HEAD_TRACKER__
#define __HEADER_OCULUS_HEAD_TRACKER__

#include <OVR.h>
#include <OVR_CAPI.h>

#include <core/scene/VRHeadset.hpp>
#include <core/asset/base/Mesh.hpp>
#include <core/asset/base/Material.hpp>
#include <core/util/SharedRef.hpp>

namespace sn {
namespace oculus {

// TODO Rename Rift
class HeadTracker : public sn::VRHeadset
{
public:
    SN_ENTITY(sn::oculus::HeadTracker, sn::VRHeadset)

    HeadTracker();

    void onReady() override;

    const Mesh * getEyeDistortionMesh(u32 eyeIndex);

    virtual void onRenderEye(
        Entity * sender, 
        Material * effectMaterial, 
        Vector2u sourceSize, 
        IntRect targetViewport
    ) override;

    // No special serialization needed at the moment

protected:
    ~HeadTracker();

    void onUpdate() override;

private:
    void makeDistortionMesh(Mesh & out_mesh, u32 agnosticEyeType);

private:
    ovrHmd m_hmd;
    ovrFrameTiming m_frameTiming;
    bool m_isFirstUpdate;
    SharedRef<Mesh> m_eyeDistortionMeshes[2];
    SharedRef<Material> m_eyeMaterials[2];
    ovrEyeRenderDesc m_eyeDesc[2];
    f32 m_lastYaw;

};

} // namespace oculus
} // namespace sn

#endif // __HEADER_OCULUS_HEAD_TRACKER__

