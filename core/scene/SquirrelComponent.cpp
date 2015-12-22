#include "../app/Application.h"
#include "SquirrelComponent.h"

namespace sn
{

SN_OBJECT_IMPL(SquirrelComponent)

SquirrelComponent::SquirrelComponent() : Component()
{
}

SquirrelComponent::~SquirrelComponent()
{
}

void SquirrelComponent::serializeState(Variant & o, const SerializationContext & context)
{
    // TODO
}

void SquirrelComponent::unserializeState(const Variant & o, const SerializationContext & context)
{
    // Script
    std::string classPath = o["class"].getString();
    if (!classPath.empty())
    {
        // TODO should be context.squirrelVM
        HSQUIRRELVM vm = Application::get().getScriptManager().getVM();

        if (m_instance.create(vm, classPath))
        {
            // Set the "entity" member
            if (getEntity().pushScriptObject(vm))
            {
                HSQOBJECT entityObj;
                sq_getstackobj(vm, -1, &entityObj);
                m_instance.setMember("entity", entityObj);
                sq_pop(vm, 1); // pop entityObj
            }

			// Call onCreate
			m_instance.callMethod("onCreate");
        }
    }

    // TODO Unserialize script members

}

} // namespace sn

