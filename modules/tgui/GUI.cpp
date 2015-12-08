#include <core/system/SystemGUI.h>
#include <core/asset/AssetDatabase.h>
#include <core/util/Profiler.h>
#include <core/scene/Scene.h>

#include <modules/render/entities/RenderManager.h>

#include "GUI.h"
#include "DrawBatch.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
GUI::GUI():
	m_defaultTheme(nullptr),
	r_captureControl(nullptr),
    r_focusControl(nullptr),
    m_clearMask(sn::render::SNR_CLEAR_NONE)
{
    m_defaultTheme = new Theme();
}

//------------------------------------------------------------------------------
GUI::~GUI()
{
    delete m_defaultTheme;
}

//------------------------------------------------------------------------------
void GUI::onReady()
{
    // Setup event filter
    u32 events[] {
        SN_EVENT_MOUSE_DOWN,
        SN_EVENT_MOUSE_UP,
        SN_EVENT_MOUSE_MOVED,
        SN_EVENT_MOUSE_ENTERED,
        SN_EVENT_MOUSE_LEFT,
        SN_EVENT_MOUSE_WHEEL_MOVED,
        SN_EVENT_KEY_DOWN,
        SN_EVENT_KEY_UP,
        SN_EVENT_WINDOW_RESIZED
    };
    for (u32 i = 0; i < sizeof(events) / sizeof(u32); ++i)
    {
        m_eventFilter[events[i]] = true;
    }

    // Fill the window
    sn::Window * window = SystemGUI::get().getWindowByID(getWindowID());
    if (window)
    {
        sn::Vector2u size = window->getClientSize();
        setLocalClientBounds(sn::IntRect::fromPositionSize(0, 0, size.x(), size.y()));
        layoutChildren();
    }

    listenToSystemEvents();
}

//------------------------------------------------------------------------------
const Theme & GUI::getTheme() const
{
    if (m_theme.isNull())
    {
        SN_ASSERT(m_defaultTheme != nullptr, "Cannot get a theme now");
        return *m_defaultTheme;
    }
    return *m_theme.get();
}

//------------------------------------------------------------------------------
void GUI::draw(sn::render::VideoDriver & driver)
{
    SN_BEGIN_PROFILE_SAMPLE_NAMED("TGUI draw");

    const Theme & theme = getTheme();

    sn::render::Material * themeMaterial = theme.getMaterial();
    if (themeMaterial)
    {
        Scene * scene = getScene();
        if (scene)
        {
            auto * renderManager = scene->getChild<sn::render::RenderManager>();
            if (renderManager)
            {
                u32 winId = getWindowID();
                sn::render::RenderScreen * screen = renderManager->getScreenByWindowId(winId);
                if (screen)
                {
                    Vector2u screenSize = screen->getSize();

                    Matrix4 projection, view;

                    projection.loadOrtho2DProjection(0, 0, static_cast<f32>(screenSize.x()), static_cast<f32>(screenSize.y()), -1, 100);
                    // TODO This translation quickfixes Matrix4 not handling assymetric ortho frustrum, please fix it
                    view.setTranslation(sn::Vector3f(-static_cast<f32>(screenSize.x()) / 2, -static_cast<f32>(screenSize.y()) / 2, 0));

                    driver.makeCurrent(*screen);

                    // Clear?
                    if (m_clearMask)
                    {
                        if (m_clearMask & sn::render::SNR_CLEAR_COLOR)
                            driver.clearColor(m_clearColor);
                        driver.clearTarget(m_clearMask);
                    }

                    driver.setViewport(0, 0, screenSize.x(), screenSize.y());

                    DrawBatch batch(driver);
                    batch.setProjectionMatrix(projection);
                    batch.setViewMatrix(view);
                    batch.setMaterial(*themeMaterial);

                    onDraw(batch);
                    batch.flush();
                }
            }
        }
    }

    SN_END_PROFILE_SAMPLE();
}

//------------------------------------------------------------------------------
void GUI::setCapture(Control * captureControl)
{
	r_captureControl = captureControl;
}

//------------------------------------------------------------------------------
void GUI::setFocusedControl(Control * focusControl)
{
    r_focusControl = focusControl;
}

//------------------------------------------------------------------------------
bool GUI::onSystemEvent(const sn::Event & systemEvent)
{
    if (m_eventFilter[systemEvent.type])
    {
        tgui::Event ev;
        ev.value = systemEvent;

        if (ev.value.type == SN_EVENT_WINDOW_RESIZED)
        {
            // Fill the window
            setLocalClientBounds(sn::IntRect::fromPositionSize(0, 0, ev.value.window.width, ev.value.window.height));
            layoutChildren();
        }
        else if (r_captureControl)
        {
			r_captureControl->onEvent(ev);
        }
        else
        {
	        dispatchEventToChildren(ev);
            if (!ev.consumed && systemEvent.type == SN_EVENT_MOUSE_MOVED)
            {
                onSetCursor(ev);
            }
        }

        return ev.consumed;
    }
    return false;
}

//------------------------------------------------------------------------------
void GUI::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);

    sn::render::serialize(o["clearBits"], m_clearMask);
    sn::serialize(o["clearColor"], m_clearColor);

    // TODO Serialize theme location
}

//------------------------------------------------------------------------------
void GUI::unserializeState(const Variant & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

	std::string themeLocation;
	sn::unserialize(o["theme"], themeLocation);
    m_theme.set(getAssetBySerializedLocation<Theme>(themeLocation, ctx.getProject()));

    sn::render::unserialize(o["clearBits"], m_clearMask);
    sn::unserialize(o["clearColor"], m_clearColor);
}

} // namespace tgui


