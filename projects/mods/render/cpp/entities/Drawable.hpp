#ifndef __HEADER_SN_RENDER_DRAWABLE__
#define __HEADER_SN_RENDER_DRAWABLE__

#include <core/scene/Drawable.hpp>
#include <core/util/SharedRef.hpp>
#include <core/asset/base/Mesh.hpp>

#include "../Material.hpp"

namespace sn {
namespace render {

// TODO Rename StaticMesh
class Drawable : public sn::Drawable
{
public:
    SN_ENTITY(sn::render::Drawable, sn::Drawable)

    Drawable();

    void onDraw(IDrawContext & dc) override;

    void setMesh(Mesh * mesh);
    const Mesh * getMesh() const { return m_mesh.get(); }

    inline void setDrawOrder(s32 order) { m_drawOrder = order; }
    inline s32 getDrawOrder() const { return m_drawOrder; }

    void setMaterial(Material * material);
    inline Material * getMaterial() const { return m_material.get(); }

    virtual void serializeState(JsonBox::Value & o, const SerializationContext & context) override;
    virtual void unserializeState(JsonBox::Value & o, const SerializationContext & context) override;

private:
    ~Drawable();

private:
    SharedRef<Mesh> m_mesh;
    SharedRef<Material> m_material;
    s32 m_drawOrder;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_DRAWABLE__


