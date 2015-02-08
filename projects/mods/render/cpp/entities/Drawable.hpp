#ifndef __HEADER_SN_RENDER_DRAWABLE__
#define __HEADER_SN_RENDER_DRAWABLE__

#include <core/scene/Entity.hpp>

namespace sn {
namespace render {

class Drawable : public Entity
{
public:

    SN_ENTITY(sn::render::Drawable, sn::Entity)

private:
    std::bitset<32> m_visibilityFlags;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_DRAWABLE__


