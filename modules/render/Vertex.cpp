#include <core/types.h>
#include "Vertex.h"

namespace sn
{

std::string toString(VertexAttribute attrib)
{
    switch (attrib)
    {
    case ATTRIB_POSITION: return "Position";
    case ATTRIB_COLOR: return "Color";
    case ATTRIB_TEXCOORD: return "Texcoord";
    case ATTRIB_NORMAL: return "Normal";
    default: return "VertexAttrib[" + std::to_string((s32)attrib) + "]";
    }
}

} // namespace sn

