#ifndef __HEADER_SN_CONTEXT_WIN32__
#define __HEADER_SN_CONTEXT_WIN32__

#include "../Context.hpp"
#include <Windows.h>

namespace sn {
namespace render {

class ContextImpl
{
public:
    ContextImpl(Context & context);
    ~ContextImpl();

    inline bool isInitialized() const { return m_hrc != nullptr; }
    bool makeCurrent(bool isCurrent);

    void swapBuffers();

private:

    //HGLRC createMinimalContext();
    void createContext(HGLRC sharedContext, ContextSettings & settings);

    Context & r_context;
    HGLRC m_hrc;
    HDC m_dc;
};

} // namespace render
} // namespace sn

#endif //__HEADER_SN_CONTEXT_WIN32__

