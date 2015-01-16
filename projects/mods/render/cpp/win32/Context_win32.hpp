#ifndef __HEADER_SN_CONTEXT_WIN32__
#define __HEADER_SN_CONTEXT_WIN32__

#include "../Context.hpp"
#include <Windows.h>

namespace sn
{

class ContextImpl
{
public:
	ContextImpl(Context & context, const Window & win);
	~ContextImpl();

	inline bool isInitialized() const { return m_hrc != nullptr; }
	bool makeCurrent();

private:

	// https://www.opengl.org/discussion_boards/showthread.php/173031-Easy-OpenGL-context-creation
	static HGLRC glCreateMinimalContext(HWND hwnd);
	static HGLRC glCreateContext(HWND hwnd, HGLRC sharedContext, int majorVersion, int minorVersion, int multiSampleMode = 0);

	Context & r_context;
	HGLRC m_hrc;
	HDC m_dc;
};

} // namespace sn

#endif //__HEADER_SN_CONTEXT_WIN32__

