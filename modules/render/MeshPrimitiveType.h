#ifndef __HEADER_SN_RENDER_MESHPRIMITIVETYPE__
#define __HEADER_SN_RENDER_MESHPRIMITIVETYPE__

#include <string>
#include <core/types.h>

namespace sn
{

enum MeshPrimitiveType
{
    // Basic primitives
    SN_MESH_POINTS = 0,
    SN_MESH_LINES = 1,
    SN_MESH_TRIANGLES = 2,

    // Emulated primitives
    SN_MESH_QUADS = 3
};

std::string toString(MeshPrimitiveType pt);

} // namespace sn

#endif // __HEADER_SN_RENDER_MESHPRIMITIVETYPE__

