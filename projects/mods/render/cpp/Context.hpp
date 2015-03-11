/*
Context.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_CONTEXT__
#define __HEADER_SN_CONTEXT__

#include <core/system/gui/Window.hpp>
#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "ContextSettings.hpp"

namespace sn {
namespace render {

class ContextImpl;

class Context
{
public:
    // TODO What if the window is closed by the user?
    Context(Window & owner, ContextSettings settings = ContextSettings());
    ~Context();

    bool makeCurrent(bool isCurrent=true);

    inline Window * getWindow() const { return r_window; }

    const ContextSettings & getSettings() const { return m_settings; }

    void useProgram(const ShaderProgram * shader = nullptr);

    void clearTarget();
    void setViewport(u32 x, u32 y, u32 width, u32 height);

    void drawMesh(const Mesh & mesh);

    // Platform-specific
    void swapBuffers();

private:
    // Platform-specific
    void initImpl();
    void deinitImpl();

    ContextImpl * m_impl;
    Window * r_window;
    ContextSettings m_settings;
};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_CONTEXT__


