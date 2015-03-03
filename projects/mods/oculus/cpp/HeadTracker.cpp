#include "HeadTracker.hpp"
#include <core/scene/Scene.hpp>

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
    // SnowfeetEngine crashes at the very end of ModOculus unloading code (when destructors of Module ends),
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
        setUpdatable(true);
    }
    else
    {
        SN_ERROR("Couldn't create OVR HMD.");
        getScene()->quit();
    }

    m_isFirstUpdate = true;
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
            SN_LOG("yaw=" << yaw << ", pitch=" << pitch << ", roll=" << roll);
        }

        ovrHmd_EndFrameTiming(m_hmd);
        ovrHmd_BeginFrameTiming(m_hmd, 0);
    }
}

} // namespace oculus
} // namespace sn


