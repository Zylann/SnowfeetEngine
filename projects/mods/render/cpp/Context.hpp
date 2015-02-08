﻿/*
Context.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_CONTEXT__
#define __HEADER_SN_CONTEXT__

#include <core/system/gui/Window.hpp>
#include "ShaderProgram.hpp"
#include "Mesh.hpp"

namespace sn {
namespace render {

class ContextImpl;

struct ContextSettings
{
    u32 majorVersion;
    u32 minorVersion;

    ContextSettings() :
        majorVersion(3),
        minorVersion(3)
    {}
};

class Context
{
public:
    // TODO What if the window is closed by the user?
    Context(Window & owner, ContextSettings settings = ContextSettings());
    ~Context();

    bool makeCurrent();

    inline Window * getWindow() const { return r_window; }

    const ContextSettings & getSettings() const { return m_settings; }

    void useProgram(const ShaderProgram * shader = nullptr);

    void clearTarget();

    void drawMesh(const Mesh & mesh);

    // Platform-specific
    void swapBuffers();

private:
    // Platform-specific
    void initImpl(const Window & owner);
    void deinitImpl();

    ContextImpl * m_impl;
    Window * r_window;
    ContextSettings m_settings;
};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_CONTEXT__


