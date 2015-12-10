/*
Context.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_GLCONTEXT__
#define __HEADER_SN_GLCONTEXT__

#include <core/system/Window.h>
#include <core/util/NonCopyable.h>

#include <modules/render/Mesh.h>

#include "ShaderProgram.h"
#include "GLContextSettings.h"
#include "ClearBits.h"
#include "BlendMode.h"

namespace sn
{

class GLContextImpl;

/// \brief Holds a valid rendering context.
class SN_RENDER_API GLContext : public NonCopyable
{
public:
    // TODO What if the window is closed by the user?

    /// \brief Creates a render context.
    /// \param window (optional): external window to attach the context to
    /// \note Settings might change to the closest match during construction, for compatibility reasons.
    GLContext(
        GLContextSettings settings = GLContextSettings(), 
        Window * window = nullptr, 
        GLContext * sharedContext = nullptr
    );

    ~GLContext();

    bool makeCurrent(bool isCurrent=true);

    inline Window * getWindow() const { return r_window; }

    const GLContextSettings & getSettings() const { return m_settings; }

    // Platform-specific
    //void swapBuffers();
    GLContextImpl * getImpl() const { return m_impl; }

private:
    // Platform-specific
    void initImpl(GLContext * sharedContext);
    void deinitImpl();

private:
    GLContextImpl * m_impl;
    Window * r_window;
    GLContextSettings m_settings;
};

} // namespace sn

#endif // __HEADER_SN_GLCONTEXT__


