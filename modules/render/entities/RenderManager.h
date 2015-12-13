#ifndef __HEADER_SN_RENDER_RENDERMANAGER__
#define __HEADER_SN_RENDER_RENDERMANAGER__

#include <core/scene/Entity.h>
#include "../RenderScreen.h"

namespace sn
{

class Camera;

// TODO This should be a service (SN_EF_SERVICE?), not an entity with SN_EF_STICKY
/// \brief Manages rendering inside one scene.
/// Contains RenderScreens (final viewports) and RenderSteps to draw on them.
class SN_RENDER_API RenderManager : public Entity
{
public:

    SN_OBJECT(sn::RenderManager, sn::Entity);

    static const char * TAG;

    RenderManager();
    ~RenderManager();

    void renderAllTaggedCameras(VideoDriver & driver, const std::string & tag);

    void onReady() override;
    void onUpdate() override;

	bool onSystemEvent(const sn::Event & event) override;

    RenderScreen * getScreenByWindowId(u32 windowID);

    void serializeState(sn::Variant & o, const SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const SerializationContext & ctx) override;

private:
    void render(VideoDriver & driver);
    void renderCamera(VideoDriver & driver, Camera & camera);

    RenderScreen * addScreen(u32 windowID=-1, const WindowParams * winParams = nullptr);
    void removeScreen(u32 windowID, bool showError=true);

    void onWindowClosed(u32 windowID);
    void onWindowResized(const Event & ev);
    void onScreenResized(u32 windowID, u32 width, u32 height);

private:
    std::unordered_map<u32, RenderScreen*> m_screens;
    sn::Mesh * m_effectQuad;

};

} // namespace sn

#endif // __HEADER_SN_RENDER_RENDERMANAGER__

