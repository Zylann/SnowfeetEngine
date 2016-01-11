#include "gl_check.h"
#include <GL/glew.h>
#include "GLContext.h"

namespace sn
{

// TODO Create a main OpenGL context that will be shared will all others
// (A context must be created for each system window).

//namespace
//{
//	Context * g_sharedContext = nullptr;
//    u32 g_contextCount = 0;
//}

//------------------------------------------------------------------------------
GLContext::GLContext(GLContextSettings settings, Window * window, GLContext * sharedContext):
    m_impl(nullptr),
    r_window(window),
    m_settings(settings)
{
	SN_LOG("Creating render context " << this << ": " << settings.toString() << " (window: " << window << ")");

    //++g_contextCount;
    //if (g_contextCount == 1)
    //{
    //    SN_ASSERT(g_sharedContext == nullptr, "Invalid state: global context is already initialized");
    //    g_sharedContext = new Context(settings);
    //}

    initImpl(sharedContext);
}

//------------------------------------------------------------------------------
GLContext::~GLContext()
{
    deinitImpl();
    SN_LOG("Destroying context " << this << " (window: " << (r_window) << ")");
    //--g_contextCount;
    //if (g_contextCount == 0)
    //{
    //    SN_ASSERT(g_sharedContext != nullptr, "Invalid state: global context is not initialized");
    //    delete g_sharedContext;
    //}
}

} // namespace sn

