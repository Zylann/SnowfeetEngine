/*
Shader.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_RENDER_SHADER__
#define __HEADER_SN_RENDER_SHADER__

#include <core/asset/Asset.hpp>

namespace sn {
namespace render {

class SN_API Shader : public Asset
{
public:

    SN_ASSET("shader");

    Shader() {}
    ~Shader() {}

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_SHADER__

