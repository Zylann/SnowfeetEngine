﻿/*
Window.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <core/util/assert.h>
#include <core/util/typecheck.h>
#include "Window.h"
//#include "WindowContainer.h"
#include "SystemGUI.h"

namespace sn
{

//------------------------------------------------------------------------------
Window::Window(SystemGUI & manager, WindowParams params):
    r_manager(manager),
    m_handle(nullptr),
    //m_parent(nullptr),
    m_impl(nullptr)
{
    bool isFirst = r_manager.getWindowCount() == 0;

    r_manager.refWindow(*this);
    if (params.style & SN_WS_CENTERED)
    {
        Vector2u desktopSize = SystemGUI::getDesktopSize();
        params.rect.x() = (static_cast<s32>(desktopSize.x()) - params.rect.width()) / 2;
        params.rect.y() = (static_cast<s32>(desktopSize.y()) - params.rect.height()) / 2;
    }

    onCreate(params, isFirst);

    if (params.parent)
    {
        Window * pwin = r_manager.getWindowByHandle(params.parent);
        //SN_ASSERT(pwin != nullptr && pwin->isContainer(), "Window: invalid parent (" << (u32)pwin << ")");
        //m_parent = checked_cast<WindowContainer*>(pwin);
    }

    //if (m_parent)
    //{
    //    m_parent->onAddChild(this);
    //}
}

//------------------------------------------------------------------------------
Window::~Window()
{
    //if (m_parent)
    //{
    //    m_parent->onRemoveChild(this);
    //}

    onDestroy();

    r_manager.unrefWindow(*this);
}

//------------------------------------------------------------------------------
bool Window::isOpen() const
{
    return m_handle != 0;
}

//------------------------------------------------------------------------------
Vector2u Window::getClientSize() const
{
    IntRect clientRect = getClientRect();
    return Vector2u(clientRect.width(), clientRect.height());
}

//------------------------------------------------------------------------------
void Window::setFullscreen(bool fullscreen, u32 monitorID)
{
    // TODO implement fullscreen
    SN_WARNING("Window::setFullscreen() is not implemented yet");
}

//------------------------------------------------------------------------------
void Window::applyParams(WindowParams params)
{
    setTitle(params.title);
    if (params.style & SN_WS_FULLSCREEN)
    {
        setFullscreen(true);
    }
    else
    {
        setClientRect(params.rect);
    }
}

} // namespace sn


