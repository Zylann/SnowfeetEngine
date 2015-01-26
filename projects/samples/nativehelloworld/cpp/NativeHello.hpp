#ifndef __HEADER_SN_SAMPLE_NATIVEHELLO__
#define __HEADER_SN_SAMPLE_NATIVEHELLO__

#include <core/scene/Entity.hpp>

namespace hello
{

class NativeHello : public sn::Entity
{
public:
    
    SN_ENTITY(hello::NativeHello, sn::Entity)

    void onReady() override;

};

} // namespace hello

#endif // __HEADER_SN_SAMPLE_NATIVEHELLO__

