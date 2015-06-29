#ifndef __HEADER_SN_DRIVER__
#define __HEADER_SN_DRIVER__

#include <core/reflect/Object.h>

namespace sn
{

/// \brief Marker interface for driver classes.
/// Drivers are sets of functions implemented by modules and accessible anywhere in C++.
class SN_API IDriver : public Object
{
public:
    SN_OBJECT(sn::IDriver, sn::Object)
};

} // namespace sn

#endif // __HEADER_SN_DRIVER__

