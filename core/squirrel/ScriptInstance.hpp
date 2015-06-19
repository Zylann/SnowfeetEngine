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
    static bool createRef(HSQUIRRELVM vm, const std::string & fullClassName, HSQOBJECT * out_obj, bool callConstructor=true);
    static bool createNoRef(HSQUIRRELVM vm, const std::string & fullClassName, HSQOBJECT * out_obj, bool callConstructor=true);

    bool create(HSQUIRRELVM vm, const std::string & fullClassName, bool callConstructor=true);

    Variant getProperty(const std::string & name);

    bool hasMethod(const std::string & methodName);
    bool callMethod(const std::string & methodName);

    void setObject(HSQUIRRELVM vm, HSQOBJECT obj);
    
};

} // namespace sn

#endif // __HEADER_SN_SCRIPTINSTANCE__

