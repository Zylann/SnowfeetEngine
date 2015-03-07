#ifndef __HEADER_SNR_GLEXTENSIONS__
#define __HEADER_SNR_GLEXTENSIONS__

namespace sn {
namespace render {

/// \brief Calling this function ensures that all OpenGL extensions are loaded,
/// allowing to use higher OpenGL versions.
bool ensureGLExtensions();

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_GLEXTENSIONS__

