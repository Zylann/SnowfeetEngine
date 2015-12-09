#ifndef __HEADER_SN_RENDER_GL_CHECK__
#define __HEADER_SN_RENDER_GL_CHECK__

namespace sn
{

#ifdef SN_BUILD_DEBUG
#define glCheck(call) call; glCheckError(__FILE__, __LINE__);
#else
#define glCheck(call) call
#endif

void glCheckError(const char * file, unsigned int line);

} // namespace sn

#endif // __HEADER_SN_RENDER_GL_CHECK__

