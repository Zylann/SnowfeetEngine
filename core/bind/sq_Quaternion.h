#ifndef __HEADER_SN_SQ_QUATERNION__
#define __HEADER_SN_SQ_QUATERNION__

#include <squirrel.h>
#include <core/math/Quaternion.h>

namespace sn
{

inline bool getQuaternionAsEuler(HSQUIRRELVM vm, const u32 i_base, Quaternion & out)
{
    f32 x, y, z;
    if (SQ_FAILED(sq_getfloat(vm, i_base, &x)) ||
        SQ_FAILED(sq_getfloat(vm, i_base+1, &y)) ||
        SQ_FAILED(sq_getfloat(vm, i_base+2, &z)))
    {
        return false;
    }
    out.setFromEuler(x, y, z);
	return true;
}

} // namespace sn

#endif // __HEADER_SN_SQ_QUATERNION__

