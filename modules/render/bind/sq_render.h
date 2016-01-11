#ifndef __HEADER_SN_RENDER_SQ_BINDING__
#define __HEADER_SN_RENDER_SQ_BINDING__

#include <squirrel.h>
#include <modules/render/entities/MeshEntity.h>
#include <modules/render/entities/Camera.h>

namespace sn
{

void bindDrawable(HSQUIRRELVM vm);
void bindMeshEntity(HSQUIRRELVM vm);
void bindCamera(HSQUIRRELVM vm);
void bindMaterial(HSQUIRRELVM vm);

void bindRenderModule(HSQUIRRELVM vm);

} // namespace sn

namespace squirrel
{
    template <> inline SQUserPointer getTypeTag<sn::Drawable>()    { return sn::bindDrawable; }
    template <> inline SQUserPointer getTypeTag<sn::MeshEntity>()  { return sn::bindMeshEntity; }
    template <> inline SQUserPointer getTypeTag<sn::Camera>()      { return sn::bindCamera; }
    template <> inline SQUserPointer getTypeTag<sn::Material>()    { return sn::bindMaterial; }
}

#endif // __HEADER_SN_RENDER_SQ_BINDING__
