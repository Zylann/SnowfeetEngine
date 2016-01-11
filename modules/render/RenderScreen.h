#ifndef __HEADER_SNR_RENDERSCREEN__
#define __HEADER_SNR_RENDERSCREEN__

#include <core/system/Window.h>
#include <modules/render/GLContext.h>

namespace sn
{

class RenderScreenImpl;
class VideoDriver;

/// \brief Encapsulates a window that is used as rendering target
class SN_RENDER_API RenderScreen : public NonCopyable
{
public:
    RenderScreen(Window & window);
    ~RenderScreen();

    void createSharedContext(GLContext & mainContext, GLContextSettings settings);

    /// \brief Gets the context attached to this window. Can be null.
    GLContext * getContext() const { return m_context; }

    bool makeCurrent(GLContext & context);

    Window & getWindow() const { return r_window; }

    const Vector2u getSize();

    //---------------------------
    // Platform-specific
    //---------------------------

    /// \brief Swaps the buffers of the window (double-buffering)
    void swapBuffers();

private:
    // Platform-specific
    void initImpl();
    void deinitImpl();
    bool makeCurrentImpl(GLContext & context);

private:
    /// \brief Context created for the window
    GLContext * m_context;
    /// \brief Window displaying the rendered frames
    Window & r_window;
    /// \brief Platform-specific implementation
    RenderScreenImpl * m_impl;

};

} // namespace sn

#endif // __HEADER_SNR_RENDERSCREEN__

