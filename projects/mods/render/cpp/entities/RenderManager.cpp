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
    {
        delete m_context;
    }
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
    listenToSystemEvents();

    // Get or create window in which we'll render the final image
    Window * win = SystemGUI::get().getWindowByID(0);
    if (win == nullptr)
        win = SystemGUI::get().createWindow();

    // Create rendering context
    if (win)
    {
        m_context = new Context(*win);
        m_context->makeCurrent();

        //IntRect rect = win->getClientRect();
        //m_lastScreenSize = Vector2u(rect.width(), rect.height());
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
    if (m_context == nullptr)
        return false;

	switch (event.type)
	{
	case SN_EVENT_WINDOW_RESIZED:
        onScreenResized(event.window.width, event.window.height);
		break;

	default:
		break;
	}

	// The events are not consumed by default
	return false;
}

//------------------------------------------------------------------------------
void RenderManager::onScreenResized(u32 width, u32 height)
{
    //m_lastScreenSize = Vector2u(width, height);

    //auto cameras = getScene()->getTaggedEntities(Camera::TAG);
    //for (auto it = cameras.begin(); it != cameras.end(); ++it)
    //{
    //    Entity * e = *it;
    //    if (e->isInstanceOf<Camera>())
    //    {
    //        Camera & cam = *(Camera*)e;
    //        if (cam.getRenderTarget() == nullptr)
    //        {
    //            cam.onTargetResized(width, height);
    //        }
    //    }
    //    else
    //    {
    //        SN_WARNING("Entity tagged Camera is not a Camera: " << e->toString());
    //    }
    //}
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

    // Display rendered surface
    m_context->swapBuffers();
}

//------------------------------------------------------------------------------
void RenderManager::renderCamera(Camera & camera)
{
    IntRect viewport = camera.getPixelViewport();

    // If the camera has effects
    if (camera.getEffectCount() > 0)
    {
        RenderTexture * rt = camera.getEffectBuffer(0);
        // Bind its first effect framebuffer as target of scene rendering
        RenderTexture::bind(rt);
        // Use filled viewport
        Vector2u size = rt->getSize();
        m_context->setViewport(0, 0, size.x(), size.y());
    }
    else
    {
        // Or bind the target directly
        RenderTexture::bind(camera.getRenderTarget());
        // Set final viewport
        m_context->setViewport(viewport);
    }

    // Clear?
    switch (camera.getClearMode())
    {
    case SNR_CLEAR_COLOR:
        m_context->clearColor(camera.getClearColor());
        break;

    default:
        // Don't clear
        break;
    }

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

    const Matrix4 & viewMatrix = camera.getGlobalMatrix();
    Matrix4 modelViewMatrix;

    // Draw them
    for (auto it = sortedDrawables.begin(); it != sortedDrawables.end(); ++it) 
    {
        const Drawable & d = **it;
        const Mesh * mesh = d.getMesh();
        if (mesh)
        {
            // If the drawable has a material, apply it
            Material * material = d.getMaterial();
            if (material)
            {
                m_context->setDepthTest(material->isDepthTest());

                if (material->getShader())
                {
                    ShaderProgram * shader = material->getShader();
                    m_context->useProgram(material->getShader());

                    modelViewMatrix.setByProduct(d.getGlobalMatrix(), viewMatrix);

                    shader->setParam("u_Projection", camera.getProjectionMatrix().values(), true);
                    shader->setParam("u_ModelView", modelViewMatrix.values(), false);

                    material->apply();
                }
            }

            // Draw the mesh
            m_context->drawMesh(*mesh);

            m_context->useProgram(nullptr);
        }
    }

    // If the camera has effects
    if (camera.getEffectCount() > 0)
    {
        // Create the default quad on which we'll render images

        Mesh * quad = new Mesh();

        quad->setPrimitiveType(SN_MESH_QUADS);

        quad->addPosition(-1, -1, 0);
        quad->addPosition( 1, -1, 0);
        quad->addPosition( 1,  1, 0);
        quad->addPosition(-1,  1, 0);

        quad->addTexCoord(0, 0);
        quad->addTexCoord(1, 0);
        quad->addTexCoord(1, 1);
        quad->addTexCoord(0, 1);

        quad->recalculateIndexes();

        RenderTexture * targetBuffer = camera.getEffectBuffer(0);
        RenderTexture * sourceBuffer = camera.getEffectBuffer(1);

        // Apply effects
        for (u32 i = 0; i < camera.getEffectCount(); ++i)
        {
            // Swap source and target:
            // Source becomes what we rendered before, target is where we'll draw with the added effect
            std::swap(sourceBuffer, targetBuffer);

            // If it's the last effect
            if (i + 1 == camera.getEffectCount())
            {
                // Bind the final target
                RenderTexture::bind(camera.getRenderTarget());
                // With the final viewport
                m_context->setViewport(viewport);
            }
            else
            {
                // Bind the next effect buffer
                RenderTexture::bind(targetBuffer);
            }

            // Apply effect material
            const Camera::Effect & effect = camera.getEffect(i);
            Material & material = *effect.material.get();
            if (material.getShader())
            {
                ShaderProgram * shader = material.getShader();
                m_context->useProgram(shader);

                // No projection, no modelview. Everything is [-1, 1].

                material.setTexture("u_MainTexture", sourceBuffer);
                material.apply();
            }

            Mesh * effectMesh = effect.mesh.get();
            if (effectMesh == nullptr)
                effectMesh = quad;

            // Draw
            m_context->drawMesh(*effectMesh);

            m_context->useProgram(nullptr);
        }

        quad->release();
    }

    // Go back to screen
    RenderTexture::bind(0);

}

} // namespace render
} // namespace sn

