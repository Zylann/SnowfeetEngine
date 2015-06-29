#ifndef __HEADER_SN_IDRAWCONTEXT__
#define __HEADER_SN_IDRAWCONTEXT__

#include <core/asset/base/Mesh.h>
#include <core/asset/base/Material.h>
#include <core/math/Rect.h>

namespace sn
{

/// \brief Simple and generic drawing interface to draw with code from anywhere.
class IDrawContext
{
public:
    virtual ~IDrawContext() {}

    /// \brief Sets the material that will be used for the next draw calls
    virtual void setMaterial(Material & mat) = 0;

    /// \brief Stretches rendering to a sub-rectangle of the current draw target
    virtual void setViewport(const IntRect & pixelRect) = 0;

    /// \brief Draws raw geometry
    virtual void drawMesh(const Mesh & mesh) = 0;

    /// \brief Sets the model matrix used for the next draw calls
    virtual void setModelMatrix(const Matrix4 & model) = 0;

    /// \brief Sets the view matrix used for the next draw calls
    virtual void setViewMatrix(const Matrix4 & view) = 0;

    /// \brief Sets the projection matrix used for the next draw calls
    virtual void setProjectionMatrix(const Matrix4 & projection) = 0;

    /// \brief Sets the normal matrix used to shade the objects from normals
    virtual void setNormalMatrix(const Matrix4 & m) = 0;

    /// \brief Clips rendering to a sub-rectangle of the current draw target
    virtual void setScissor(IntRect rect) = 0;

    /// \brief Disables scissor clipping
    virtual void disableScissor() = 0;

    /// \brief Sets where the next draw calls should draw on.
    //virtual void setTargetTexture(RenderTexture * rt) = 0;

};

} // namespace sn

#endif // __HEADER_SN_IDRAWCONTEXT__

