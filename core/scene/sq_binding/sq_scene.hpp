#ifndef __HEADER_SN_SQ_SCENE__
#define __HEADER_SN_SQ_SCENE__

#include <core/scene/Entity.hpp>
#include <core/scene/Scene.hpp>
#include <core/squirrel/bind_tools.hpp>

//SN_SQ_DEFINE_REFCOUNTED_VARTYPE(sn::Entity)
//SN_SQ_DEFINE_REFCOUNTED_VARTYPE(sn::Scene)

namespace sn
{

void registerScene(HSQUIRRELVM vm);
void registerEntity(HSQUIRRELVM vm);

} // namespace sn

#endif // __HEADER_SN_SQ_SCENE__


