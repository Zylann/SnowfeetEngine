#include "../scene/Entity3D.hpp"
#include "sq_core.h"
#include "../squirrel/bind_macros.h"

using namespace squirrel;

namespace sn
{
	namespace
	{
        template <u32 i_base>
		inline bool getVector3fAs3Numbers(HSQUIRRELVM vm, Vector3f & out)
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

        CURRENT_CLASS(Entity3D)

        BEGIN_METHOD(setPosition)
            Vector3f v;
            if (!getVector3fAs3Numbers<2>(vm, v))
                return sq_throwerror(vm, "Expected 3 numbers");
			self->setPosition(v);
			return 0;
		END_METHOD
        
		// TODO Better float popping
		// TODO Bind math primitives!
	}


void bindEntity3D(HSQUIRRELVM vm)
{
	ScriptableObject::bindBase<Entity3D>(vm)
		.setMethod("setPosition", setPosition, -1);
}

} // namespace sn


