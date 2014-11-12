#ifndef __HEADER_SYSTEM_GUI__
#define __HEADER_SYSTEM_GUI__

#include <core/system/gui/Window.hpp>
#include <core/util/NonCopyable.hpp>

#include <map>

namespace sn
{

/// \brief Singleton manager of all system windows and events
class SystemGUI : public NonCopyable
{
public:

    // Get singleton
    static SystemGUI & get();

    SystemGUI();
    ~SystemGUI();

    Window * getWindowByHandle(WindowHandle h);
    Window * getWindowByID(u32 id);
    bool getWindowID(Window * win, u32 * id);

    inline u32 getWindowCount() const { return m_windows.size(); }

    //--------------------------------
    // Internal
    //--------------------------------

    void refWindow(Window & win);
    void unrefWindow(Window & win);

private:

    void onCreate();
    void onDestroy();

    u32 makeWindowID();

    std::map<Window*,u32> m_windows;
    u32 m_nextWindowID;
};

} // namespace sn

#endif // __HEADER_SYSTEM_GUI__

