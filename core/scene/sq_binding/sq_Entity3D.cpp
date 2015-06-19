#include "sq_scene.hpp"
#include "../Entity3D.hpp"
#include "../../squirrel/bind_macros.h"

namespace sn
{
	namespace
	{
		inline bool isNumber(HSQUIRRELVM vm, s32 i)
		{
			SQObjectType t = sq_gettype(vm, i);
			return t == OT_INTEGER || t == OT_FLOAT;
		}
        
        template <u32 i_base>
		inline Vector3f getVector3f(HSQUIRRELVM vm)
		{
			if (isNumber(vm, i_base) && 
				isNumber(vm, i_base+1) &&
				isNumber(vm, i_base+2))
			{
				return Vector3f(
					getFloat(vm, i_base), 
					getFloat(vm, i_base+1),
					getFloat(vm, i_base+2)
				);
			}
			else
			{
				SN_ERROR("Expected 3 numbers");
				return Vector3f(0, 0, 0);
			}
		}

		DEFINE_CLASS(Entity3D)

		BEGIN_METHOD(setPosition)
			self->setPosition(getVector3f<2>(vm));
			return 0;
		END_METHOD

		// TODO Better float popping
		// TODO Bind math primitives!
	}


void registerEntity3D(HSQUIRRELVM vm)
{
	ScriptableObject::bindBase<Entity3D>(vm)
		.setMethod("setPosition", setPosition, -1);
}

} // namespace sn


