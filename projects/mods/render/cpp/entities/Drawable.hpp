#ifndef __HEADER_SN_RENDER_DRAWABLE__
#define __HEADER_SN_RENDER_DRAWABLE__

#include <core/scene/Entity.hpp>
#include <core/util/SharedRef.hpp>
//#include <mods/render/cpp/Mesh.hpp>
#include "../Mesh.hpp"
#include "../Material.hpp"

namespace sn {
namespace render {

class Drawable : public Entity
{
public:
    SN_ENTITY(sn::render::Drawable, sn::Entity)

    static const std::string TAG;

    Drawable();

    void onReady() override;

    void setMesh(Mesh * mesh);
    const Mesh * getMesh() const { return m_mesh.get(); }

    inline void setDrawOrder(s32 order) { m_drawOrder = order; }
    inline s32 getDrawOrder() const { return m_drawOrder; }

    void setMaterial(Material * material);
    inline Material * getMaterial() const { return m_material.get(); }

private:
    ~Drawable();

private:
    std::bitset<32> m_visibilityFlags;
    SharedRef<Mesh> m_mesh;
    SharedRef<Material> m_material;
    s32 m_drawOrder;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_DRAWABLE__


