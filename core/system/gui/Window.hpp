#ifndef __HEADER_SN_WINDOW__
#define __HEADER_SN_WINDOW__

#include <string>
#include <set>

#include <core/math/Rect.hpp>
#include <core/system/gui/Layout.hpp>

namespace sn
{

//------------------------------------------------------------------------------
typedef void* WindowHandle;

//------------------------------------------------------------------------------
enum WindowStyle
{
    SN_WS_BORDER = 1,
    SN_WS_CLOSABLE = 2,
    SN_WS_MAXIMIZABLE = 4,
    SN_WS_MINIMIZABLE = 8,
    SN_WS_SHOWN = 16,
    SN_WS_FULLSCREEN = 32,
    SN_WS_CAPTION = 64,

    _SN_WS_DEFAULT = SN_WS_BORDER
        | SN_WS_CLOSABLE
        | SN_WS_MAXIMIZABLE
        | SN_WS_MINIMIZABLE
        | SN_WS_SHOWN
        | SN_WS_CAPTION
};

//------------------------------------------------------------------------------
struct WindowParams
{
    WindowParams(
        WindowHandle parent_,
        const std::string title_,
        IntRect rect_,
        unsigned int style = _SN_WS_DEFAULT
    ) :
        parent(parent_),
        rect(rect_),
        title(title_),
        style(style)
    {}

    WindowHandle parent;
    IntRect rect;
    std::string title;
    unsigned int style;
    std::string winClass; // Used on windows
};

//------------------------------------------------------------------------------
class Window // : public IWindow
{
public:

    Window(WindowParams params = WindowParams(NULL, "Window", IntRect(0, 0, 0, 0)));
    ~Window();

    inline bool isCreated() const { return m_handle != 0; }

    inline void show() { setVisible(true); }
    void setVisible(bool visible);
    
    inline WindowHandle getHandle() const { return m_handle; };

    void setTitle(const std::string & title);
    inline const std::string & getTitle() const { return m_title; }

    void setRect(IntRect rect);
    IntRect getRect() const;

    void setClientRect(IntRect rect);
    IntRect getClientRect() const;

    inline unsigned int getChildCount() const { return m_children.size(); }

    template <class Layout_T>
    Layout_T & setLayout()
    {
        if (m_layout)
            delete m_layout;
        m_layout = new Layout_T(this);
        return *m_layout;
    }

    void clearLayout();

    inline Layout * getLayout() const { return m_layout; }

    static Window * getByHandle(WindowHandle handle);

private:

    //friend class Layout;
    
    void onAddChild(Window * child);
    void onRemoveChild(Window * child);

    void create(WindowParams params);
    void destroy();

    void invalidateAll();

    WindowHandle m_handle;
    std::string m_title;

    Window* m_parent;
    std::set<Window*> m_children;

    Layout * m_layout;
};

} // namespace sn

#endif // __HEADER_SN_WINDOW__


