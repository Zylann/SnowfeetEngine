#ifndef __HEADER_SNR_VERTEX__
#define __HEADER_SNR_VERTEX__

#include <string>

namespace sn {
namespace render {

// for use in OpenGL3
enum VertexAttribute
{
    SNR_ATTRIB_POSITION = 0,
    SNR_ATTRIB_COLOR,
    SNR_ATTRIB_TEXCOORD,
    SNR_ATTRIB_NORMAL,
    //...
    SNR_ATTRIB_COUNT // Keep last
};

std::string toString(VertexAttribute st);

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_VERTEX__

