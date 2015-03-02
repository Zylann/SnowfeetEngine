#ifndef __HEADER_OCULUS_HEAD_TRACKER__
#define __HEADER_OCULUS_HEAD_TRACKER__

#include <OVR.h>
#include <OVR_CAPI.h>

#include <core/scene/Entity.hpp>

namespace sn {
namespace oculus {

class HeadTracker : public sn::Entity
{
public:
    SN_ENTITY(sn::oculus::HeadTracker, sn::Entity)

    HeadTracker();
    ~HeadTracker();

    void onReady() override;

protected:
    void onUpdate() override;

private:
    ovrHmd m_hmd;
    ovrFrameTiming m_frameTiming;
    bool m_isFirstUpdate;

};

} // namespace oculus
} // namespace sn

#endif // __HEADER_OCULUS_HEAD_TRACKER__

