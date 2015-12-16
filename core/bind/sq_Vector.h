#ifndef __HEADER_SN_SQ_VECTOR__
#define __HEADER_SN_SQ_VECTOR__

#include <squirrel.h>

namespace sn
{

inline bool getVector3fAs3Numbers(HSQUIRRELVM vm, const u32 i_base, Vector3f & out)
{
    f32 x, y, z;
    if (SQ_FAILED(sq_getfloat(vm, i_base, &x)) ||
        SQ_FAILED(sq_getfloat(vm, i_base+1, &y)) ||
        SQ_FAILED(sq_getfloat(vm, i_base+2, &z)))
    {
        return false;
    }
    out.x() = x;
    out.y() = y;
    out.z() = z;
	return true;
}

} // namespace sn

#endif // __HEADER_SN_SQ_VECTOR__

