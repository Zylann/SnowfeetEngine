#ifndef __HEADER_SNR_VERTEX__
#define __HEADER_SNR_VERTEX__

#include <string>

namespace sn
{

// for use in OpenGL3
enum VertexAttribute
{
    ATTRIB_POSITION = 0,
    ATTRIB_COLOR,
    ATTRIB_TEXCOORD,
    ATTRIB_NORMAL,
    //...
    ATTRIB_COUNT // Keep last
};

std::string toString(VertexAttribute st);

} // namespace sn

#endif // __HEADER_SNR_VERTEX__

