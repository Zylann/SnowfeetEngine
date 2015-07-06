#ifndef __HEADER_SN_RENDER_SQ_BINDING__
#define __HEADER_SN_RENDER_SQ_BINDING__

#include <squirrel.h>

namespace sn {
namespace render {

void bindDrawable(HSQUIRRELVM vm);
void bindCamera(HSQUIRRELVM vm);

void bindRenderModule(HSQUIRRELVM vm);

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_SQ_BINDING__


