#ifndef __HEADER_WINDOW_CONTAINER__
#define __HEADER_WINDOW_CONTAINER__

#include <set>
#include <core/system/gui/Window.hpp>

namespace sn
{

/// \brief Window that can have embedded child windows
class WindowContainer : public Window
{
public:

    WindowContainer(SystemGUI & gui, WindowParams params = WindowParams(NULL, "WindowContainer"));
    ~WindowContainer();

    inline unsigned int getChildCount() const { return m_children.size(); }

    inline Layout * getLayout() const { return m_layout; }

    template <class Layout_T>
    Layout_T & setLayout()
    {
        if (m_layout)
            delete m_layout;
        m_layout = new Layout_T(this);
        return *m_layout;
    }

    void clearLayout();

    virtual bool isContainer() const { return true; }

private:

    friend class Window;

    void onAddChild(Window * child);
    void onRemoveChild(Window * child);

    std::set<Window*> m_children;

    Layout * m_layout;

};

} // namespace sn

#endif // __HEADER_WINDOW_CONTAINER__

