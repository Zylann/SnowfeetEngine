#ifndef __HEADER_SYSTEM_GUI__
#define __HEADER_SYSTEM_GUI__

#include <core/system/gui/Window.hpp>
#include <core/util/NonCopyable.hpp>

#include <set>

namespace sn
{

class SystemGUI : public NonCopyable
{
public:

    // Get singleton
    static SystemGUI & get();

    SystemGUI();
    ~SystemGUI();

    void refWindow(Window & win);
    void unrefWindow(Window & win);

    Window * getWindowByHandle(WindowHandle h);

    inline u32 getWindowCount() const { return m_windows.size(); }

private:

    void onCreate();
    void onDestroy();

    std::set<Window*> m_windows;

};

} // namespace sn

#endif // __HEADER_SYSTEM_GUI__

