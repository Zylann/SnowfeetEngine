#include "HeadTracker.hpp"
#include <core/scene/Scene.h>
#include <core/scene/Entity3D.h>
#include <core/asset/AssetDatabase.h>

#include <Kernel/OVR_Math.h>

namespace OVR
{
    // Due to be removed once the functionality is in the SDK
    void UtilFoldExtraYawIntoTimewarpMatrix(Matrix4f * timewarpMatrix, Quatf eyePose, Quatf extraQuat)
    {
        timewarpMatrix->M[0][1] = -timewarpMatrix->M[0][1];
        timewarpMatrix->M[0][2] = -timewarpMatrix->M[0][2];
        timewarpMatrix->M[1][0] = -timewarpMatrix->M[1][0];
        timewarpMatrix->M[2][0] = -timewarpMatrix->M[2][0];
        Quatf newtimewarpStartQuat = eyePose * extraQuat * (eyePose.Inverted())*(Quatf(*timewarpMatrix));
        *timewarpMatrix = Matrix4f(newtimewarpStartQuat);
        timewarpMatrix->M[0][1] = -timewarpMatrix->M[0][1];
        timewarpMatrix->M[0][2] = -timewarpMatrix->M[0][2];
        timewarpMatrix->M[1][0] = -timewarpMatrix->M[1][0];
        timewarpMatrix->M[2][0] = -timewarpMatrix->M[2][0];
    }
}

namespace sn {
namespace oculus {

//------------------------------------------------------------------------------
/// \brief Gets euler angles from an OVR rotation applying the engine's graphic conventions
void getEulerAnglesFromOVR(OVR::Quat<float> & q, Vector3f & euler)
{
    q.GetEulerAngles<OVR::Axis_X, OVR::Axis_Y, OVR::Axis_Z, OVR::Rotate_CCW, OVR::Handed_L>(&euler.x(), &euler.y(), &euler.z());
}

//------------------------------------------------------------------------------
HeadTracker::HeadTracker():
    m_ovrHmd(nullptr),
    m_lastYaw(0),
    m_debugHmdType(ovrHmd_None),
    m_debug(false)
{
    m_abstractEyes[0].tag = "VR_LeftEye";
    m_abstractEyes[1].tag = "VR_RightEye";
}

//------------------------------------------------------------------------------
HeadTracker::~HeadTracker()
{
    if (m_ovrHmd)
    {
        ovrHmd_Destroy(m_ovrHmd);
    }
    ovr_Shutdown();
}

//------------------------------------------------------------------------------
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
        if (!m_debug)
            m_ovrHmd = ovrHmd_Create(0);
    }

    if (!m_ovrHmd && m_debugHmdType != ovrHmd_None)
    {
        if (!m_debug)
            SN_ERROR("Couldn't create OVR HMD from plugged device. Fallback on debug HMD (" << sn::oculus::toString(m_debugHmdType) << ")");
        m_ovrHmd = ovrHmd_CreateDebug(m_debugHmdType);
    }

    if (m_ovrHmd)
    {
        ovrHmd_ConfigureTracking(m_ovrHmd, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position, 0);
        setUpdatable(true);

        // Initialize eye descriptions
        m_ovrEyeDesc[0] = ovrHmd_GetRenderDesc(m_ovrHmd, ovrEye_Left, m_ovrHmd->DefaultEyeFov[0]);
        m_ovrEyeDesc[1] = ovrHmd_GetRenderDesc(m_ovrHmd, ovrEye_Right, m_ovrHmd->DefaultEyeFov[1]);
        for (u32 i = 0; i < 2; ++i)
        {
            const ovrEyeRenderDesc & ovrEyeDesc = m_ovrEyeDesc[i];
            VRHeadset::EyeDescription & eyeDesc = m_abstractEyes[i];

            eyeDesc.fov = Fov(
                ovrEyeDesc.Fov.LeftTan, 
                ovrEyeDesc.Fov.RightTan, 
                ovrEyeDesc.Fov.UpTan, 
                ovrEyeDesc.Fov.DownTan
            );

            eyeDesc.viewport = IntRect::fromPositionSize(
                ovrEyeDesc.DistortedViewport.Pos.x,
                ovrEyeDesc.DistortedViewport.Pos.y,
                ovrEyeDesc.DistortedViewport.Size.w,
                ovrEyeDesc.DistortedViewport.Size.h
            );
        }

        // Make eye distortion meshes

        // HACK: the Oculus module contains two important mesh files for each eye that will be
        // loaded by the asset system, and made available to camera effects in rendering modules.
        // They contain dummy quads = no lens warp...
        // BUT, when we come to these lines of code, the two meshes will be updated
        // to their real geometry, because we can only get it after an Oculus has
        // been started, which is not necessarily the case when the application launches.

        // TODO Don't hardcode the names, we should create a dynamic asset instead
        // TODO Find a better design where we can have several models of headsets simultaneously

        auto & database = AssetDatabase::get();
        std::string projectName = "oculus";

        m_abstractEyes[0].distortionMesh.set(database.getAsset<Mesh>(projectName, "left_eye"));
        m_abstractEyes[1].distortionMesh.set(database.getAsset<Mesh>(projectName, "right_eye"));

        for (u32 i = 0; i < 2; ++i)
        {
            Mesh * mesh = m_abstractEyes[i].distortionMesh.get();
            if (mesh)
            {
                makeDistortionMesh(*mesh, i);
            }
        }

        m_abstractEyes[0].effectMaterial.set(database.getAsset<Material>(projectName, "left_eye_mat"));
        m_abstractEyes[1].effectMaterial.set(database.getAsset<Material>(projectName, "right_eye_mat"));

        // Must be called before any call to ovrHmd_BeginFrameTiming
        ovrHmd_ResetFrameTiming(m_ovrHmd, 0);

        m_ovrFrameTiming = ovrHmd_BeginFrameTiming(m_ovrHmd, 0);    
    }
    else
    {
        SN_ERROR("Couldn't create OVR HMD.");
        //getScene()->quit();
    }

    const char* err = ovrHmd_GetLastError(m_ovrHmd);
    if (err && err[0] != '\0')
    {
        SN_ERROR(err);
    }
}

//------------------------------------------------------------------------------
Vector2u HeadTracker::getPreferredFramebufferSize(EyeIndex eyeIndex)
{
    if (m_ovrHmd)
    {
        ovrEyeType eyeType = (ovrEyeType)eyeIndex;
        ovrSizei ovrSize = ovrHmd_GetFovTextureSize(m_ovrHmd, eyeType, m_ovrEyeDesc[eyeType].Fov, 1.0);
        return Vector2u(ovrSize.w, ovrSize.h);
    }
    else
    {
        // HMD not initialized
        return Vector2u(0, 0);
    }
}

//------------------------------------------------------------------------------
void HeadTracker::onRenderEye(Entity * sender, VRHeadset::EyeIndex abstractEyeIndex, Material * effectMaterial, Vector2u sourceSize, IntRect targetViewport)
{
    Material * material = effectMaterial;
    if (material)
    {
        ovrTrackingState trackingState = ovrHmd_GetTrackingState(m_ovrHmd, m_ovrFrameTiming.ScanoutMidpointSeconds);
        //if (trackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
        //{
            // Get head pose
            OVR::Posef pose = trackingState.HeadPose.ThePose;
            Vector3f euler;
            getEulerAnglesFromOVR(pose.Rotation, euler);
            //pose.Rotation.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);

            // Get eye poses
            ovrPosef temp_EyeRenderPose[2];
            ovrVector3f useHmdToEyeViewOffset[2] = {
                m_ovrEyeDesc[0].HmdToEyeViewOffset,
                m_ovrEyeDesc[1].HmdToEyeViewOffset
            };
            ovrHmd_GetEyePoses(m_ovrHmd, 0, useHmdToEyeViewOffset, temp_EyeRenderPose, NULL);

            // Get current eye index
            ovrEyeType eye = (abstractEyeIndex == VRHeadset::EYE_RIGHT ? ovrEye_Right : ovrEye_Left);
            const ovrEyeRenderDesc & eyeDesc = m_ovrEyeDesc[eye];

            // Update UV parameters
            ovrSizei rtSize = { sourceSize.x(), sourceSize.y() };
            ovrRecti camViewport = { 0, 0, sourceSize.x(), sourceSize.y() };
            //ovrRecti camViewport = { 0, 0, targetViewport.width(), targetViewport.height() };
            ovrVector2f UVScaleOffset[2];
            ovrHmd_GetRenderScaleAndOffset(eyeDesc.Fov, rtSize, camViewport, UVScaleOffset);
            material->setParam("u_EyeToSourceUVScale", UVScaleOffset[0].x, UVScaleOffset[0].y);
            material->setParam("u_EyeToSourceUVOffset", UVScaleOffset[1].x, UVScaleOffset[1].y);

            // Update timewarp parameters

            ovrPosef eyePose = temp_EyeRenderPose[eye];

            ovrMatrix4f timeWarpMatrices[2];
            OVR::Quatf extraYawSinceRender = OVR::Quatf(OVR::Vector3f(0, 1, 0), euler.y() - m_lastYaw);
            ovrHmd_GetEyeTimewarpMatrices(m_ovrHmd, eye, eyePose, timeWarpMatrices);// , debugTimeAdjuster);

            // Due to be absorbed by a future SDK update
            OVR::UtilFoldExtraYawIntoTimewarpMatrix((OVR::Matrix4f *)&timeWarpMatrices[0], eyePose.Orientation, extraYawSinceRender);
            OVR::UtilFoldExtraYawIntoTimewarpMatrix((OVR::Matrix4f *)&timeWarpMatrices[1], eyePose.Orientation, extraYawSinceRender);

            timeWarpMatrices[0] = ((OVR::Matrix4f)timeWarpMatrices[0]).Transposed();
            timeWarpMatrices[1] = ((OVR::Matrix4f)timeWarpMatrices[1]).Transposed();
            material->setParam("u_EyeRotationStart", (f32*)&timeWarpMatrices[0]);
            material->setParam("u_EyeRotationEnd", (f32*)&timeWarpMatrices[1]);
        //}
    }
}

//------------------------------------------------------------------------------
// TODO Use a callback from a rendering module instead of onUpdate
void HeadTracker::onUpdate()
{
    if (m_ovrHmd == nullptr)
        return;
    // At this point we assume the headset is plugged and working

    // We're inside the main loop already so we'll assume that the frame begins after our update and ends at the end of it.

    ovrTrackingState trackingState = ovrHmd_GetTrackingState(m_ovrHmd, m_ovrFrameTiming.ScanoutMidpointSeconds);
    if (trackingState.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
    {
        // Get head pose
        OVR::Posef pose = trackingState.HeadPose.ThePose;

        // Update parent's position
        Entity * targetEntity = getParent();
        if (targetEntity && targetEntity->isInstanceOf<Entity3D>())
        {
            Entity3D & target3D = *(Entity3D*)targetEntity;
            OVR::Quat<float> q = pose.Rotation;
            // Note: Y and X axes need to be inverted to match the coordinate system
            target3D.setRotation(Quaternion(q.w, -q.x, -q.y, q.z));
        }

        // Memorize last head/eye variables
        Vector3f euler;
        getEulerAnglesFromOVR(pose.Rotation, euler);
        m_lastYaw = euler.y();
    }

    ovrHmd_EndFrameTiming(m_ovrHmd);
    ovrHmd_BeginFrameTiming(m_ovrHmd, 0);
}

//------------------------------------------------------------------------------
void HeadTracker::makeDistortionMesh(Mesh & out_mesh, u32 agnosticEyeType)
{
    ovrEyeType eye = (agnosticEyeType == 1 ? ovrEye_Right : ovrEye_Left);
    const ovrEyeRenderDesc & eyeDesc = m_ovrEyeDesc[agnosticEyeType];

    ovrDistortionMesh meshData;
    float overrideEyeOffset = 0.f;

    ovrHmd_CreateDistortionMesh(m_ovrHmd, eye, eyeDesc.Fov, ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp, &meshData);

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

    f32 offsetX = agnosticEyeType == 0 ? 1.f : -1.f;
    for (u32 i = 0; i < meshData.VertexCount; ++i)
    {
        const ovrDistortionVertex & v = meshData.pVertexData[i];

        // Note: positions are pre-transformed here because LibOVR gives us a mesh
        // that is either completely in [-1,0] or [0,1] on X, and currently camera effects for eyes
        // work both in [-1,1]. However...
        // TODO This pre-transform could be avoided if we could render the eyes in the same render target
        positions[i] = Vector3f(v.ScreenPosNDC.x * 2.f + offsetX, v.ScreenPosNDC.y, 0);
        timeWarpFactor[i] = v.TimeWarpFactor;
        vignetteFactor[i] = v.VignetteFactor;
        tanEyeAnglesR[i] = Vector2f(v.TanEyeAnglesR.x, v.TanEyeAnglesR.y);
        tanEyeAnglesG[i] = Vector2f(v.TanEyeAnglesG.x, v.TanEyeAnglesG.y);
        tanEyeAnglesB[i] = Vector2f(v.TanEyeAnglesB.x, v.TanEyeAnglesB.y);
    }

    ovrHmd_DestroyDistortionMesh(&meshData);

    out_mesh.clear();
    out_mesh.setPositions(&positions[0], positions.size());
    out_mesh.setCustomFloats(0, &vignetteFactor[0], vignetteFactor.size());
    out_mesh.setCustomFloats(1, &timeWarpFactor[0], timeWarpFactor.size());
    //out_mesh.setUV(&tanEyeAnglesR[0], tanEyeAnglesR.size()); // ?
    out_mesh.setCustomVec2Buffer(0, &tanEyeAnglesR[0], tanEyeAnglesR.size());
    out_mesh.setCustomVec2Buffer(1, &tanEyeAnglesG[0], tanEyeAnglesG.size());
    out_mesh.setCustomVec2Buffer(2, &tanEyeAnglesB[0], tanEyeAnglesB.size());
    out_mesh.setTriangleIndices(&indices[0], indices.size());

    out_mesh.recalculateBounds();

}

//------------------------------------------------------------------------------
void HeadTracker::unserializeState(const Variant & o, const SerializationContext & context)
{
    sn::VRHeadset::unserializeState(o, context);
    sn::unserialize(o["debug"], m_debug);
    sn::oculus::unserialize(o["debugHmdType"], m_debugHmdType);
}

//------------------------------------------------------------------------------
void HeadTracker::serializeState(Variant & o, const SerializationContext & context)
{
    sn::VRHeadset::serializeState(o, context);
    sn::serialize(o["debug"], m_debug);
    sn::oculus::serialize(o["debugHmdType"], m_debugHmdType);
}

//------------------------------------------------------------------------------
void serialize(Variant & o, ovrHmdType hmdType)
{
    o.setString(toString(hmdType));
}

//------------------------------------------------------------------------------
void unserialize(const Variant & o, ovrHmdType & out_hmdType)
{
    if (o.isString())
    {
        std::string s = o.getString();
        //std::transform(s.begin(), s.end(), s.begin(), ::tolower);

        if (!s.empty())
        {
            if (s == "DK1")
            {
                out_hmdType = ovrHmd_DK1;
            }
            else if (s == "DK2")
            {
                out_hmdType = ovrHmd_DK2;
            }
            else if (s == "DKHD")
            {
                out_hmdType = ovrHmd_DKHD;
            }
            else if (s == "none" || s == "None")
            {
                out_hmdType = ovrHmd_None;
            }
        }
    }
}

//------------------------------------------------------------------------------
std::string toString(ovrHmdType type)
{
    switch (type)
    {
    case ovrHmd_DK1: return "DK1"; break;
    case ovrHmd_DK2: return "DK2"; break;
    case ovrHmd_DKHD: return "DKHD"; break;
    case ovrHmd_None: return "None"; break;
    default: return "Other"; break;
    }
}

} // namespace oculus
} // namespace sn


