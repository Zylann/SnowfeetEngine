#include "HeadTracker.hpp"
#include <core/scene/Scene.hpp>
#include <core/scene/Entity3D.hpp>

#include <Kernel/OVR_Math.h>

namespace sn {
namespace oculus {

// TODO This is mostly test code, cleanup things when tests work well

HeadTracker::HeadTracker():
    m_hmd(nullptr)
{
}

HeadTracker::~HeadTracker()
{
    if (m_hmd)
    {
        ovrHmd_Destroy(m_hmd);
    }
    ovr_Shutdown();
}

void HeadTracker::onReady()
{
    // TODO FIXME There is a strange bug when ovr_Initialize is called with neither runtime nor headset plugged in.
    // SnowfeetEngine crashes at the very end of ModOculus unloading code (when destructor of Module ends),
    // suggesting that the memory has been corrupted somehow.
    // It should be good to cleanly cancel things if required conditions aren't fulfilled.

    if (!ovr_Initialize())
    {
        SN_ERROR("Couldn't initialize OVR.");
    }
    else
    {
        m_hmd = ovrHmd_Create(0);
    }

    if (m_hmd)
    {
        ovrHmd_ConfigureTracking(m_hmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
        setUpdatable(true);

        // Get eye distortion meshes

        Mesh * leftEyeDistortion = new Mesh();
        Mesh * rightEyeDistortion = new Mesh();

        makeDistortionMesh(*leftEyeDistortion, 0);
        makeDistortionMesh(*leftEyeDistortion, 1);

        m_eyeDistortionMeshes[0].set(leftEyeDistortion);
        m_eyeDistortionMeshes[1].set(rightEyeDistortion);
    }
    else
    {
        SN_ERROR("Couldn't create OVR HMD.");
        getScene()->quit();
    }

    const char* err = ovrHmd_GetLastError(m_hmd);
    if (err)
    {
        SN_ERROR(err);
    }

    m_isFirstUpdate = true;
}

const Mesh * HeadTracker::getEyeDistortionMesh(u32 eyeIndex)
{
    SN_ASSERT(eyeIndex == 0 || eyeIndex == 1, "Invalid eye index");
    return m_eyeDistortionMeshes[eyeIndex].get();
}

void HeadTracker::onUpdate()
{
    // At this point we assume the headset is plugged and working

    if (m_isFirstUpdate)
    {
        // We're inside the main loop already so we'll assume that the frame begins after our update and ends at the end of it.
        // TODO onBeforeRender() and onAfterRender() callbacks for each VR cameras
        m_frameTiming = ovrHmd_BeginFrameTiming(m_hmd, 0);
        m_isFirstUpdate = false;
    }
    else
    {
        ovrTrackingState trackingState = ovrHmd_GetTrackingState(m_hmd, m_frameTiming.ScanoutMidpointSeconds);
        if (trackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
        {
            OVR::Posef pose = trackingState.HeadPose.ThePose;
            f32 yaw, pitch, roll;
            pose.Rotation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);

            //SN_LOG("yaw=" << yaw << ", pitch=" << pitch << ", roll=" << roll);

            Entity * targetEntity = getParent();
            if (targetEntity && targetEntity->isInstanceOf<Entity3D>())
            {
                Entity3D & target3D = *(Entity3D*)targetEntity;
                target3D.setRotation(Quaternion(pitch * math::RAD2DEG, yaw * math::RAD2DEG, roll * math::RAD2DEG));
            }
        }

        ovrHmd_EndFrameTiming(m_hmd);
        ovrHmd_BeginFrameTiming(m_hmd, 0);
    }
}

void HeadTracker::makeDistortionMesh(Mesh & out_mesh, u32 agnosticEyeType)
{
    ovrEyeType eye = (agnosticEyeType == 1 ? ovrEye_Right : ovrEye_Left);
    ovrEyeRenderDesc eyeDesc = ovrHmd_GetRenderDesc(m_hmd, eye, m_hmd->DefaultEyeFov[eye]);

    ovrDistortionMesh meshData;
    float overrideEyeOffset = 0.f;

    ovrHmd_CreateDistortionMesh(m_hmd, eye, eyeDesc.Fov, ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp, &meshData);

    // Note: we cannot use direct pointers here because data types have not the same size,
    // And are not layout in arrays but in structures

    std::vector<u32> indices(meshData.IndexCount);
    std::vector<Vector3f> positions(meshData.VertexCount);
    std::vector<f32> timeWarpFactor(meshData.VertexCount);
    std::vector<f32> vignetteFactor(meshData.VertexCount);
    std::vector<Vector2f> tanEyeAnglesR(meshData.VertexCount);
    std::vector<Vector2f> tanEyeAnglesG(meshData.VertexCount);
    std::vector<Vector2f> tanEyeAnglesB(meshData.VertexCount);

    for (u32 i = 0; i < indices.size(); ++i)
    {
        indices[i] = meshData.pIndexData[i];
    }

    for (u32 i = 0; i < meshData.VertexCount; ++i)
    {
        const ovrDistortionVertex & v = meshData.pVertexData[i];

        positions[i] = Vector3f(v.ScreenPosNDC.x, v.ScreenPosNDC.y, 0);
        timeWarpFactor[i] = v.TimeWarpFactor;
        vignetteFactor[i] = v.VignetteFactor;
        tanEyeAnglesR[i] = Vector2f(v.TanEyeAnglesR.x, v.TanEyeAnglesR.y);
        tanEyeAnglesG[i] = Vector2f(v.TanEyeAnglesG.x, v.TanEyeAnglesG.y);
        tanEyeAnglesB[i] = Vector2f(v.TanEyeAnglesB.x, v.TanEyeAnglesB.y);
    }

    ovrHmd_DestroyDistortionMesh(&meshData);

    out_mesh.clear();
    out_mesh.setPositions(&positions[0], positions.size());
    out_mesh.setCustomFloats(0, &timeWarpFactor[0], timeWarpFactor.size());
    out_mesh.setCustomFloats(1, &timeWarpFactor[0], timeWarpFactor.size());
    //out_mesh.setCustomVec2Buffer(0, &tanEyeAnglesR[0], tanEyeAnglesR.size());
    //out_mesh.setCustomVec2Buffer(1, &tanEyeAnglesG[0], tanEyeAnglesG.size());
    //out_mesh.setCustomVec2Buffer(2, &tanEyeAnglesB[0], tanEyeAnglesB.size());
    out_mesh.setTriangleIndices(&indices[0], indices.size());

}

} // namespace oculus
} // namespace sn


