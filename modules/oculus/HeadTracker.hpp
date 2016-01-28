#ifndef __HEADER_OCULUS_HEAD_TRACKER__
#define __HEADER_OCULUS_HEAD_TRACKER__

#include <core/util/SharedRef.h>

#include <modules/render/entities/VRHeadset.h>
#include <modules/render/Mesh.h>
#include <modules/render/Material.h>

#include <OVR.h>
#include <OVR_CAPI.h>

namespace sn {
namespace oculus {

namespace DistortionMesh
{
    enum Attribute
    {
        POSITION = VertexAttribute::USE_POSITION,
        TIMEWARP_FACTOR = VertexAttribute::USE_CUSTOM + 1,
        VIGNETTE_FACTOR = VertexAttribute::USE_CUSTOM + 2,
        TAN_EYE_ANGLES_R = VertexAttribute::USE_CUSTOM + 3,
        TAN_EYE_ANGLES_G = VertexAttribute::USE_CUSTOM + 4,
        TAN_EYE_ANGLES_B = VertexAttribute::USE_CUSTOM + 5
    };
}

// TODO Rename RiftController
class HeadTracker : public sn::VRHeadset
{
public:
    SN_OBJECT

    HeadTracker();

    void onReady() override;

    void onRenderEye(
        Entity * sender, 
        VRHeadset::EyeIndex abstractEyeIndex,
        sn::Material * effectMaterial, 
        Vector2u sourceSize, 
        IntRect targetViewport
    ) override;

    Vector2u getPreferredFramebufferSize(EyeIndex eye) override;

    void unserializeState(const Variant & o, const SerializationContext & context) override;
    void serializeState(Variant & o, const SerializationContext & context) override;

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
    ovrHmdType m_debugHmdType;
    bool m_debug;

};

void serialize(Variant & o, ovrHmdType hmdType);
void unserialize(const Variant & o, ovrHmdType & out_hmdType);

std::string toString(ovrHmdType type);

} // namespace oculus
} // namespace sn

#endif // __HEADER_OCULUS_HEAD_TRACKER__

