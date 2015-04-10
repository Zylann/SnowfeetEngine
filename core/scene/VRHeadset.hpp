#ifndef __HEADER_SN_VRHEADSET__
#define __HEADER_SN_VRHEADSET__

#include <core/scene/Entity.hpp>
#include <core/math/Rect.hpp>
#include <core/math/Matrix4.hpp>
#include <core/math/Fov.hpp>
#include <core/asset/base/Material.hpp>

namespace sn
{

/// \brief Base class for VR headsets controllers.
/// \warning Wether this class should stay here or not is not decided yet.
/// It is here because I need to communicate between rendering and the Oculus Rift,
/// and the scripting system is not advanced enough to do it.
class SN_API VRHeadset : public Entity
{
public:
    SN_ENTITY(sn::VRHeadset, sn::Entity)

    /// \brief An eye's description, abstracted from device-specific information.
    struct AbstractEyeDescription
    {
        /// \brief The tag a camera should have to be considered as an eye
        std::string tag;
        /// \brief Field of view of the eye
        Fov fov;
    };

    /// \brief Called just before an eye camera renders an effect. This lets the material to be modified if needed.
    virtual void onRenderEye(Entity * sender, Material * effectMaterial, Vector2u sourceSize, IntRect targetViewport) = 0;

    /// \brief Gets an eye's description
    /// \param eyeIndex: index of the eye. 0 = left, 1 = right.
    const AbstractEyeDescription & getAbstractEyeDescription(u32 eyeIndex) { return m_abstractEyes[eyeIndex]; }

protected:
    AbstractEyeDescription m_abstractEyes[2];

};

} // namespace sn

#endif // __HEADER_SN_VRHEADSET__

