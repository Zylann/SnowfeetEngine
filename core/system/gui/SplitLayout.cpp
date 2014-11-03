#include <core/system/gui/SplitLayout.hpp>
#include <core/system/gui/Window.hpp>
#include <core/math/math.hpp>

namespace sn
{

//------------------------------------------------------------------------------
Layout & SplitLayout::setDirection(SplitDirection dir)
{
    m_direction = dir;
    return *this;
}

//------------------------------------------------------------------------------
Layout & SplitLayout::setFirst(Window * win)
{
    r_first = win;
    return *this;
}

//------------------------------------------------------------------------------
Layout & SplitLayout::setSecond(Window * win)
{
    r_second = win;
    return *this;
}

//------------------------------------------------------------------------------
Layout & SplitLayout::setMode(SplitMode mode, f32 ratio)
{
    m_mode = mode;
    if (m_mode == SN_SM_RATIO)
        m_ratio = math::clamp(ratio, 0.f, 1.f);
    return *this;
}

//------------------------------------------------------------------------------
void SplitLayout::update()
{
    IntRect containerRect = r_container->getClientRect();
    IntRect firstRect = r_first->getClientRect();

    s32 sep = 0;
    if (m_mode == SN_SM_RATIO)
    {
        switch (m_direction)
        {
        case SN_SD_LEFT:
            sep = (s32)((f32)containerRect.width * m_ratio);
            break;

        case SN_SD_RIGHT:
            sep = (s32)((f32)containerRect.width * (1.f-m_ratio));
            break;

        case SN_SD_UP:
            sep = (s32)((f32)containerRect.height * m_ratio);
            break;

        case SN_SD_DOWN:
            sep = (s32)((f32)containerRect.height * (1.f-m_ratio));
            break;

        default:
            break;
        }
    }
    else
    {
        switch (m_direction)
        {
        case SN_SD_LEFT:
            sep = firstRect.width;
            break;

        case SN_SD_RIGHT:
            sep = containerRect.width - firstRect.width;
            break;

        case SN_SD_UP:
            sep = firstRect.height;
            break;

        case SN_SD_DOWN:
            sep = containerRect.height - firstRect.height;
            break;

        default:
            break;
        }
    }

    IntRect newFirstRect(firstRect);
    switch (m_direction)
    {
    case SN_SD_LEFT:
        newFirstRect.x = 0;
        newFirstRect.y = 0;
        newFirstRect.height = containerRect.height;
        newFirstRect.width = sep;
        break;

    case SN_SD_RIGHT:
        newFirstRect.x = sep;
        newFirstRect.y = 0;
        newFirstRect.width = containerRect.width - sep;
        newFirstRect.height = containerRect.height;
        break;

    case SN_SD_UP:
        newFirstRect.x = 0;
        newFirstRect.y = 0;
        newFirstRect.width = containerRect.width;
        newFirstRect.height = sep;
        break;

    case SN_SD_DOWN:
        newFirstRect.x = 0;
        newFirstRect.y = containerRect.height - sep;
        newFirstRect.height = sep;
        newFirstRect.width = containerRect.width;
        break;

    default:
        break;
    }

    if (newFirstRect != firstRect)
    {
        r_first->setClientRect(newFirstRect);
        firstRect = newFirstRect;
    }

    if (r_container->getChildCount() >= 2)
    {
        Window * second = r_second;
        IntRect secondRect = second->getRect();

        IntRect newSecondRect = secondRect;
        switch (m_direction)
        {
        case SN_SD_LEFT:
            newSecondRect.x = firstRect.width;
            newSecondRect.y = 0;
            newSecondRect.width = containerRect.width - firstRect.width;
            newSecondRect.height = containerRect.height;
            break;

        case SN_SD_RIGHT:
            newSecondRect.x = 0;
            newSecondRect.y = 0;
            newSecondRect.width = containerRect.width - firstRect.width;
            newSecondRect.height = containerRect.height;
            break;

        case SN_SD_UP:
            newSecondRect.x = 0;
            newSecondRect.y = firstRect.height;
            newSecondRect.width = containerRect.width;
            newSecondRect.height = containerRect.height - firstRect.height;
            break;

        case SN_SD_DOWN:
            newSecondRect.x = 0;
            newSecondRect.y = 0;
            newSecondRect.width = containerRect.width;
            newSecondRect.height = containerRect.height - firstRect.height;
            break;

        default:
            break;
        }

        if (newSecondRect != secondRect)
        {
            second->setClientRect(newSecondRect);
        }
    }
}

} // namespace sn


