#include "../scene/Entity3D.hpp"
#include "sq_core.h"
#include "../squirrel/bind_macros.h"

using namespace squirrel;

namespace sn
{
	namespace
	{
        template <u32 i_base>
		inline Vector3f getVector3fAs3Numbers(HSQUIRRELVM vm)
		{
            f32 x, y, z;
            if (SQ_FAILED(sq_getfloat(vm, i_base, &x)) ||
                SQ_FAILED(sq_getfloat(vm, i_base+1, &y)) ||
                SQ_FAILED(sq_getfloat(vm, i_base+2, &z)))
            {
				SN_ERROR("Expected 3 numbers");
				return Vector3f(0, 0, 0);
            }
            return Vector3f(x, y, z);
		}

		DEFINE_CLASS(Entity3D)

		BEGIN_METHOD(setPosition)
			self->setPosition(getVector3fAs3Numbers<2>(vm));
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


