#include <core/system/gui/SystemGUI.hpp>
#include <core/scene/Scene.hpp>
#include <core/asset/AssetDatabase.hpp> // TODO Remove?

#include "RenderManager.hpp"
#include "Drawable.hpp"
#include "Camera.hpp"

namespace sn {
namespace render {

const char * RenderManager::TAG = "RenderManager";

//------------------------------------------------------------------------------
RenderManager::RenderManager() : Entity(), m_context(nullptr)
{
    // This entity is always present across scenes
    setFlag(SN_EF_STICKY, true);
}

//------------------------------------------------------------------------------
RenderManager::~RenderManager()
{
    if (m_context)
        delete m_context;
}

//------------------------------------------------------------------------------
void RenderManager::onReady()
{
    SN_ASSERT(m_context == nullptr, "Invalid state, m_context is not null");

    // TODO If two renderManager are created, just use another window ID and it might work
    SN_ASSERT(getScene()->getTaggedEntity(TAG) == nullptr, "Two RenderManagers is not supported at the moment");

    addTag(TAG);

    // Register to update manager.
    // Arbitrary values. The layer is given quite high so gameplay should be updated before.
    setUpdatable(true, 0, 32000);

    // Get or create window in which we'll render
    Window * win = SystemGUI::get().getWindowByID(0);
    if (win == nullptr)
        win = SystemGUI::get().createWindow();

    // Create rendering context
    if (win)
    {
        m_context = new Context(*win);
        m_context->makeCurrent();
    }
}

//------------------------------------------------------------------------------
void RenderManager::onUpdate()
{
    // Render!
    render();
}

//------------------------------------------------------------------------------
bool RenderManager::onSystemEvent(const sn::Event & event)
{
	switch (event.type)
	{
	case SN_EVENT_WINDOW_RESIZED:
		// TODO change viewport resolution
		break;

	default:
		break;
	}

	// The events are not consumed by default
	return false;
}

//------------------------------------------------------------------------------
// Helper
template <typename T>
T * checkTaggedType(const std::string & tag, Entity * e)
{
    const ObjectType & ot = T::__sGetObjectType();
    if (e->getObjectType() == ot)
    {
        return static_cast<T*>(e);
    }
    else
    {
        SN_ERROR("Entity " << e->toString() << " has tag " << tag << " but is not a " << ot.toString());
        return nullptr;
    }
}

//------------------------------------------------------------------------------
void RenderManager::render()
{
    if (m_context == nullptr)
        return; // Can't render

    // Erase screen
    m_context->clearTarget();

    Scene * scene = getScene();

    // Get cameras
    std::vector<Entity*> cameras = scene->getTaggedEntities(Camera::TAG);
    std::vector<Camera*> sortedCameras;
    for (auto it = cameras.begin(); it != cameras.end(); ++it)
    {
        Camera * cam = checkTaggedType<Camera>(Camera::TAG, *it);
        if (cam)
        {
            sortedCameras.push_back(cam);
        }
    }

    // Sort cameras
    std::sort(sortedCameras.begin(), sortedCameras.end(), 
        [](Camera * a, Camera * b) {
            return a->getDrawOrder() < b->getDrawOrder();
        }
    );

    // Draw scene parts viewed by cameras
    for (auto it = sortedCameras.begin(); it != sortedCameras.end(); ++it)
    {
        renderCamera(**it);
    }

    // TODO Remove this when basic testing is over
    // BEGIN TEST CODE

    Mesh mesh;
    //mesh.setPrimitiveType(SNR_PT_TRIANGLES);
    mesh.addPosition(-0.5f, -0.5f);
    mesh.addPosition(0.f, 0.5f);
    mesh.addPosition(0.5f, -0.5f);
    mesh.addColor(sn::Color(1, 0, 0));
    mesh.addColor(sn::Color(0, 1, 0));
    mesh.addColor(sn::Color(0, 0, 1));

    AssetDatabase & assets = AssetDatabase::get();
    ShaderProgram * sp = assets.getAsset<ShaderProgram>("render", "basic");
    if (sp)
    {
        m_context->useProgram(sp);
    }

    m_context->drawMesh(mesh);
    m_context->useProgram(nullptr);

    // END TEST CODE

    // Display rendered surface
    m_context->swapBuffers();
}

//------------------------------------------------------------------------------
void RenderManager::renderCamera(Camera & camera)
{
    // Get drawables
    std::vector<Entity*> drawables = getScene()->getTaggedEntities(Drawable::TAG);
    std::vector<Drawable*> sortedDrawables;
    for (auto it = drawables.begin(); it != drawables.end(); ++it)
    {
        Drawable * d = checkTaggedType<Drawable>(Drawable::TAG, *it);
        if (d)
        {
            sortedDrawables.push_back(d);
        }
    }

    // Sort drawables
    std::sort(sortedDrawables.begin(), sortedDrawables.end(),
        [](Drawable * a, Drawable * b) {
            return a->getDrawOrder() < b->getDrawOrder();
        }
    );

    // Draw them
    for (auto it = sortedDrawables.begin(); it != sortedDrawables.end(); ++it)
    {
        const Drawable & d = **it;
        const Mesh * mesh = d.getMesh();
        if (mesh)
        {
            m_context->drawMesh(*mesh);
        }
    }

}

} // namespace render
} // namespace sn

