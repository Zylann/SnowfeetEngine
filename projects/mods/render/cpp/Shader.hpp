/*
Shader.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SHADER__
#define __HEADER_SN_SHADER__

#include <core/asset/Asset.hpp>

namespace sn
{

class SN_API Shader : public Asset
{
public:

    SN_ASSET("shader");

    Shader() {}
    ~Shader() {}

};

} // namespace sn

#endif // __HEADER_SN_SHADER__

