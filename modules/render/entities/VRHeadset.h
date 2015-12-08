#ifndef __HEADER_SN_VRHEADSET__
#define __HEADER_SN_VRHEADSET__

#include <core/scene/Entity.h>
#include <core/math/Rect.h>
#include <core/math/Matrix4.h>
#include <core/math/Fov.h>
#include <core/util/SharedRef.h>

#include <modules/render/Material.h>
#include <modules/render/Mesh.h>

namespace sn
{

/// \brief Base class for VR headsets controllers.
class SN_RENDER_API VRHeadset : public Entity
{
public:
    SN_ENTITY(sn::VRHeadset, sn::Entity)

    enum EyeIndex
    {
        EYE_LEFT = 0,
        EYE_RIGHT = 1,
        EYE_COUNT = 2
    };

    /// \brief An eye's description, abstracted from device-specific information.
    struct EyeDescription
    {
        /// \brief The tag a camera should have to be considered as an eye
        std::string tag;
        /// \brief Field of view of the eye
        Fov fov;
        /// \brief Distortion mesh if any
        SharedRef<Mesh> distortionMesh;
        /// \brief Post-processing effect material if any
        SharedRef<sn::render::Material> effectMaterial;
        /// \brief Part of the HMD screen used to render the eye, in pixels.
        IntRect viewport;
    };

    /// \brief Called just before an eye camera renders an effect. This lets the material to be modified if needed.
    virtual void onRenderEye(Entity * sender, EyeIndex abstractEyeIndex, sn::render::Material * effectMaterial, Vector2u sourceSize, IntRect targetViewport) = 0;

    /// \brief Gets which is the preferred resolution to render the undistorted image for a given eye.
    /// \param eye: which eye is concerned
    /// \return resolution in pixels
    virtual Vector2u getPreferredFramebufferSize(EyeIndex eye) = 0;

    /// \brief Gets an eye's description
    /// \param eyeIndex: index of the eye. 0 = left, 1 = right.
    /// \return device-independent eye description
    const EyeDescription & getAbstractEyeDescription(EyeIndex eyeIndex) { return m_abstractEyes[eyeIndex]; }

protected:
    EyeDescription m_abstractEyes[EYE_COUNT];

};

} // namespace sn

#endif // __HEADER_SN_VRHEADSET__

