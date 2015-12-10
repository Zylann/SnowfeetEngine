#ifndef __HEADER_SNR_VIDEODRIVER__
#define __HEADER_SNR_VIDEODRIVER__

#include <core/app/Driver.h>
#include <modules/render/Texture.h>
#include <modules/render/RenderScreen.h>

namespace sn
{

class SN_RENDER_API VideoDriver : public sn::IDriver
{
public:
    SN_OBJECT(sn::VideoDriver, sn::IDriver)

    VideoDriver();
    ~VideoDriver();

    sn::Texture * createTexture();

    void makeCurrent(bool isCurrent = true);
    void makeCurrent(RenderScreen & screen);

    /// \brief Sets the shader that will be used for the next draw calls
    void useProgram(const ShaderProgram * shader = nullptr);

    void clearTarget(ClearMask mask = SNR_CLEAR_ALL);

    void clearColor(const Color color);

    void setDepthTest(bool enabled);

    /// \brief Stretches rendering to a sub-rectangle of the current draw target
    void setViewport(u32 x, u32 y, u32 width, u32 height);
    void setViewport(const IntRect & rect);

    /// \brief Draws raw geometry
    void drawMesh(const Mesh & mesh);

    /// \brief Clamps next draw calls to a sub-rectangle on the current render target.
    void setScissor(IntRect rect);
    /// \brief Disables scissor.
    void disableScissor();

    void setBlendMode(BlendMode mode);

private:
    GLContext * m_context;

};

} // namespace sn

#endif // __HEADER_SNR_VIDEODRIVER__

