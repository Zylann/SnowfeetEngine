#ifndef __HEADER_SN_RENDER_MESHENTITY__
#define __HEADER_SN_RENDER_MESHENTITY__

#include <core/util/SharedRef.h>

#include <modules/render/Mesh.h>
#include <modules/render/Material.h>
#include <modules/render/entities/Drawable.h>

namespace sn
{

class SN_RENDER_API MeshEntity : public Drawable
{
public:
    SN_ENTITY(sn::MeshEntity, sn::Drawable)

    MeshEntity();

    void onDraw(RenderState & state) override;

    void setMesh(Mesh * mesh);
    const Mesh * getMesh() const { return m_mesh.get(); }

    inline void setDrawOrder(s32 order) { m_drawOrder = order; }
    inline s32 getDrawOrder() const { return m_drawOrder; }

    void setMaterial(Material * material);
    inline Material * getMaterial() const { return m_material.get(); }

    virtual void serializeState(sn::Variant & o, const SerializationContext & context) override;
    virtual void unserializeState(const sn::Variant & o, const SerializationContext & context) override;

private:
    ~MeshEntity();

private:
    SharedRef<Mesh> m_mesh;
    SharedRef<Material> m_material;
    s32 m_drawOrder;

};

} // namespace sn

#endif // __HEADER_SN_RENDER_MESHENTITY__


