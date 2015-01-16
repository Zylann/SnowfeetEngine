#include "Context.hpp"

namespace sn
{

Context::Context(Window & owner):
	m_impl(nullptr),
	r_window(&owner)
{
	initImpl(owner);
}

Context::~Context()
{
	deinitImpl();
}

} // namespace sn

