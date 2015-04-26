#ifndef __HEADER_SN_RENDER_RENDERMANAGER__
#define __HEADER_SN_RENDER_RENDERMANAGER__

#include <core/scene/Entity.hpp>
#include "../Context.hpp"
#include "../RenderScreen.hpp"

namespace sn {
namespace render {

class Camera;

// TODO This should be a service (SN_EF_SERVICE?), not an entity with SN_EF_STICKY
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
    
    void serializeState(JsonBox::Value & o, const SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const SerializationContext & ctx) override;

private:
    void render();
    void renderCamera(Camera & camera);

    RenderScreen * addScreen(u32 windowID=-1, const WindowParams * winParams = nullptr);
    void removeScreen(u32 windowID, bool showError=true);
    RenderScreen * getScreen(u32 windowID);

    void onWindowClosed(u32 windowID);
    void onWindowResized(const Event & ev);
    void onScreenResized(u32 windowID, u32 width, u32 height);

private:
    Context * m_mainContext;
    std::unordered_map<u32, RenderScreen*> m_screens;

};

} // namespace render
} // namespace

#endif // __HEADER_SN_RENDER_RENDERMANAGER__

