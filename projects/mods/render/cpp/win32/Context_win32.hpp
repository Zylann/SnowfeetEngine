#ifndef __HEADER_SN_CONTEXT_WIN32__
#define __HEADER_SN_CONTEXT_WIN32__

#include "../Context.hpp"
#include <Windows.h>

namespace sn {
namespace render {

/// \cond INTERNAL

class ContextImpl : public NonCopyable
{
public:
    ContextImpl(Context & context, ContextSettings & settings, ContextImpl * sharedContext = nullptr);
    ~ContextImpl();

    inline bool isInitialized() const { return m_hrc != nullptr; }
    bool makeCurrent(bool isCurrent);

    //void swapBuffers();

    HGLRC getHandle() const { return m_hrc; }

    /// \brief Tries to set an OpenGL-compatible pixel format to the given window.
    /// \param hwnd: handle to the window
    /// \param settings: the pixel format will be based on these settings.
    /// They can be downgraded for compatibility.
    /// \return true on success, false on failure
    static bool setPixelFormat(HWND hwnd, ContextSettings & settings);

private:
    void createContext(HGLRC sharedContext, ContextSettings & settings, HWND hwnd);

private:
    Context & r_context;
    HGLRC m_hrc;
    HDC m_dc;

    HWND m_ownHwnd;
};

/// \endcond

} // namespace render
} // namespace sn

#endif //__HEADER_SN_CONTEXT_WIN32__

