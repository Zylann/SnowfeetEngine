#include "MeshPrimitiveType.h"

namespace sn
{

//------------------------------------------------------------------------------
std::string toString(MeshPrimitiveType pt)
{
    switch (pt)
    {
    case SN_MESH_POINTS: return "Points"; break;
    case SN_MESH_LINES: return "Lines"; break;
    case SN_MESH_TRIANGLES: return "Lines"; break;
    case SN_MESH_QUADS: return "Quads"; break;
    default: return "MeshPrimitiveType[" + std::to_string((s32)pt) + "]"; break;
    }
}

} // namespace sn


