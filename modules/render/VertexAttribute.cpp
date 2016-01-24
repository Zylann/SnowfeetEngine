#include "VertexAttribute.h"

namespace sn
{

std::string toString(VertexAttribute::Use use)
{
    switch (use)
    {
    case VertexAttribute::USE_POSITION: return "Position";
    case VertexAttribute::USE_COLOR: return "Color";
    case VertexAttribute::USE_TEXCOORD: return "Texcoord";
    case VertexAttribute::USE_NORMAL: return "Normal";
    default: return std::string("Attribute") + std::to_string(use);
    }
}

} // namespace sn

