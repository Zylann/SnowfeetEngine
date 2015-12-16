#ifndef __HEADER_SN_GLCONTEXT_WIN32__
#define __HEADER_SN_GLCONTEXT_WIN32__

#include "../GLContext.h"
#include <Windows.h>

namespace sn
{

/// \cond INTERNAL

class GLContextImpl : public NonCopyable
{
public:
    GLContextImpl(GLContext & context, GLContextSettings & settings, GLContextImpl * sharedContext = nullptr);
    ~GLContextImpl();

    inline bool isInitialized() const { return m_hrc != nullptr; }
    bool makeCurrent(bool isCurrent);

    //void swapBuffers();

    HGLRC getHandle() const { return m_hrc; }

    /// \brief Tries to set an OpenGL-compatible pixel format to the given window.
    /// \param hwnd: handle to the window
    /// \param settings: the pixel format will be based on these settings.
    /// They can be downgraded for compatibility.
    /// \return true on success, false on failure
    static bool setPixelFormat(HWND hwnd, GLContextSettings & settings);

private:
    void createContext(HGLRC sharedContext, GLContextSettings & settings, HWND hwnd);

private:
    GLContext & r_context;
    HGLRC m_hrc;
    HDC m_dc;

    HWND m_ownHwnd;
};

/// \endcond

} // namespace sn

#endif //__HEADER_SN_CONTEXT_WIN32__

