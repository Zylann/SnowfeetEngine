#ifndef __HEADER_SN_SCRIPTINSTANCE__
#define __HEADER_SN_SCRIPTINSTANCE__

#include <vector>
#include <core/util/Variant.hpp>
#include <core/squirrel/ScriptObject.h>

namespace sn
{

// TODO Make it inherit ScriptTable?
/// \brief Wraps a Squirrel class instance.
class SN_API ScriptInstance : public ScriptObject
{
public:
    bool create(const std::string & fullClassName);

    Variant getProperty(const std::string & name);

    bool hasMethod(const std::string & methodName);
    bool callMethod(const std::string & methodName);
    
};

} // namespace sn

#endif // __HEADER_SN_SCRIPTINSTANCE__

