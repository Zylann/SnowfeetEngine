#include "../scene/Entity3D.hpp"
#include "sq_core.h"
#include "../squirrel/bind_macros.h"
#include "sq_Vector.h"

using namespace squirrel;

namespace sn
{
	namespace
	{
        CURRENT_CLASS(Entity3D)

        BEGIN_METHOD(setPosition)
            Vector3f v;
            if (!getVector3fAs3Numbers(vm, 2, v))
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


