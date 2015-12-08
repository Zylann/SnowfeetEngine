/*
Context.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_CONTEXT__
#define __HEADER_SN_CONTEXT__

#include <core/system/Window.h>
#include <core/util/NonCopyable.h>

#include <modules/render/Mesh.h>

#include "ShaderProgram.h"
#include "ContextSettings.h"
#include "ClearBits.h"
#include "BlendMode.h"

namespace sn {
namespace render {

class ContextImpl;

/// \brief Holds a valid rendering context.
class SN_RENDER_API Context : public NonCopyable
{
public:
    // TODO What if the window is closed by the user?

    /// \brief Creates a render context.
    /// \param window (optional): external window to attach the context to
    /// \note Settings might change to the closest match during construction, for compatibility reasons.
    Context(
        ContextSettings settings = ContextSettings(), 
        Window * window = nullptr, 
        Context * sharedContext = nullptr
    );

    ~Context();

    bool makeCurrent(bool isCurrent=true);

    inline Window * getWindow() const { return r_window; }

    const ContextSettings & getSettings() const { return m_settings; }

    // Platform-specific
    //void swapBuffers();
    ContextImpl * getImpl() const { return m_impl; }

private:
    // Platform-specific
    void initImpl(Context * sharedContext);
    void deinitImpl();

private:
    ContextImpl * m_impl;
    Window * r_window;
    ContextSettings m_settings;
};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_CONTEXT__


