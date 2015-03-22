#ifndef __HEADER_SN_RENDER_RENDERMANAGER__
#define __HEADER_SN_RENDER_RENDERMANAGER__

#include <core/scene/Entity.hpp>
#include "../Context.hpp"

namespace sn {
namespace render {

class Camera;

class RenderManager : public Entity
{
public:

    SN_ENTITY(sn::render::RenderManager, sn::Entity);

    static const char * TAG;

    RenderManager();
    ~RenderManager();

    void onReady() override;
    void onUpdate() override;

	bool onSystemEvent(const sn::Event & event) override;
    
private:
    void render();
    void renderCamera(Camera & camera);

    void onScreenResized(u32 width, u32 height);

private:
    Context * m_context;

};

} // namespace render
} // namespace

#endif // __HEADER_SN_RENDER_RENDERMANAGER__

