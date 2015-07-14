#include <core/types.h>
#include "Vertex.h"

namespace sn {
namespace render {

std::string toString(VertexAttribute attrib)
{
    switch (attrib)
    {
    case SNR_ATTRIB_POSITION: return "Position";
    case SNR_ATTRIB_COLOR: return "Color";
    case SNR_ATTRIB_TEXCOORD: return "Texcoord";
    case SNR_ATTRIB_NORMAL: return "Normal";
    default: return "VertexAttrib[" + std::to_string((s32)attrib) + "]";
    }
}

} // namespace render
} // namespace sn

