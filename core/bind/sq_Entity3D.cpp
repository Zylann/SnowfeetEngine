#include "../scene/Entity3D.h"
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

        SQRESULT setPosition(HSQUIRRELVM vm)
        {
            GET_SELF();
            Vector3f v;
            if (!getVector3fAs3Numbers(vm, 2, v))
                return sq_throwerror(vm, "Expected 3 numbers");
			self->setPosition(v);
			return 0;
        }

        SQRESULT setRotation(HSQUIRRELVM vm)
        {
            GET_SELF();
            Quaternion q;
            if (!getQuaternionAsEuler(vm, 2, q))
                return sq_throwerror(vm, "Expected 3 euler angles (deg)");
            self->setRotation(q);
            return 0;
        }
        
		// TODO Bind math primitives!
	}


void bindEntity3D(HSQUIRRELVM vm)
{
    ScriptableObject::bindBase<Entity3D>(vm)
        .setMethod("setPosition", setPosition, -2)
        .setMethod("setRotation", setRotation, 4, "xnnn")
    ;
}

} // namespace sn


