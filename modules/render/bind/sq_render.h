#ifndef __HEADER_SN_RENDER_SQ_BINDING__
#define __HEADER_SN_RENDER_SQ_BINDING__

#include <squirrel.h>
#include <modules/render/entities/MeshEntity.h>
#include <modules/render/entities/Camera.h>

namespace sn {
namespace render {

void bindDrawable(HSQUIRRELVM vm);
void bindMeshEntity(HSQUIRRELVM vm);
void bindCamera(HSQUIRRELVM vm);
void bindMaterial(HSQUIRRELVM vm);

void bindRenderModule(HSQUIRRELVM vm);

} // namespace render
} // namespace sn

namespace squirrel
{
    template <> inline SQUserPointer getTypeTag<sn::render::Drawable>()    { return sn::render::bindDrawable; }
    template <> inline SQUserPointer getTypeTag<sn::render::MeshEntity>()  { return sn::render::bindMeshEntity; }
    template <> inline SQUserPointer getTypeTag<sn::render::Camera>()      { return sn::render::bindCamera; }
    template <> inline SQUserPointer getTypeTag<sn::render::Material>()    { return sn::render::bindMaterial; }
}

#endif // __HEADER_SN_RENDER_SQ_BINDING__
