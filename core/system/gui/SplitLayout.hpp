#ifndef __HEADER_SN_SPLITLAYOUT__
#define __HEADER_SN_SPLITLAYOUT__

#include <core/system/gui/Layout.hpp>
#include <core/types.hpp>

namespace sn
{

enum SplitDirection
{
    SN_SD_LEFT,
    SN_SD_RIGHT,
    SN_SD_UP,
    SN_SD_DOWN
};

enum SplitMode
{
    SN_SM_FIXED,
    SN_SM_RATIO
};

class SplitLayout : Layout
{
public:

    SplitLayout(Window*container) :
        Layout(container),
        m_direction(SN_SD_LEFT),
        r_first(nullptr),
        r_second(nullptr)
    {}

    Layout & setDirection(SplitDirection dir);
    Layout & setFirst(Window * win);
    Layout & setSecond(Window * win);
    Layout & setMode(SplitMode mode, f32 ratio = 0.5);

    void update();

private:

    SplitDirection m_direction;
    Window * r_first;
    Window * r_second;
    SplitMode m_mode;
    f32 m_ratio;
};

} // namespace sn

#endif

