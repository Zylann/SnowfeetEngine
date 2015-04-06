/*
SharedLib.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "SharedLib.hpp"

namespace sn
{

SharedLib::SharedLib():
    m_impl(nullptr)
{}

SharedLib::~SharedLib()
{
    unload();
}

} // namespace sn

