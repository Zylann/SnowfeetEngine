#ifndef __HEADER_SNR_RENDERSCREEN__
#define __HEADER_SNR_RENDERSCREEN__

#include <core/system/gui/Window.hpp>
#include "Context.hpp"

namespace sn {
namespace render {

class RenderScreenImpl;

/// \brief Encapsulates a window that is used as rendering target
class RenderScreen : public NonCopyable
{
public:
    RenderScreen(Window & window);
    ~RenderScreen();

    void createSharedContext(Context & mainContext, ContextSettings settings);

    /// \brief Gets the context attached to this window. Can be null.
    Context * getContext() const { return m_context; }

    Window & getWindow() const { return r_window; }

    //---------------------------
    // Platform-specific
    //---------------------------

    /// \brief Swaps the buffers of the window (double-buffering)
    void swapBuffers();

    static bool setCurrent(RenderScreen * screen, Context * context = nullptr);

private:
    bool makeCurrent(Context * context = nullptr);

    // Platform-specific
    void initImpl();
    void deinitImpl();
    bool makeCurrentImpl(Context & context);

private:
    /// \brief Context created for the window
    Context * m_context;
    /// \brief Window displaying the rendered frames
    Window & r_window;
    /// \brief Platform-specific implementation
    RenderScreenImpl * m_impl;

};

} // namespace sn
} // namespace render

#endif // __HEADER_SNR_RENDERSCREEN__

