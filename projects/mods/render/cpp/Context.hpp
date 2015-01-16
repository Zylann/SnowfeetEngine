/*
Context.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_CONTEXT__
#define __HEADER_SN_CONTEXT__

#include <core/system/gui/Window.hpp>

namespace sn
{

class ContextImpl;

class Context
{
public:
	// TODO What if the window is closed by the user?
	Context(Window & owner);
	~Context();

	bool makeCurrent();
	inline Window * getWindow() const { return r_window; }

private:
	void initImpl(const Window & owner);
	void deinitImpl();

	ContextImpl * m_impl;
	Window * r_window;
};

} // namespace sn

#endif // __HEADER_SN_CONTEXT__


