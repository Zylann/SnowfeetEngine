#include "../scene/Entity3D.hpp"
#include "sq_core.h"
#include "../squirrel/bind_macros.h"
#include "sq_Vector.h"
#include "sq_Quaternion.h"

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

        BEGIN_METHOD(setRotation)
            Quaternion q;
            if (!getQuaternionAsEuler(vm, 2, q))
                return sq_throwerror(vm, "Expected 3 euler angles (deg)");
            self->setRotation(q);
            return 0;
        END_METHOD
        
		// TODO Bind math primitives!
	}


void bindEntity3D(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Entity3D>(vm)
        .setMethod("setPosition", setPosition, -1)
        .setMethod("setRotation", setRotation, 3, "nnn");
}

} // namespace sn


