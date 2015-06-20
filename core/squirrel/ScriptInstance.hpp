#ifndef __HEADER_SQUIRREL_INSTANCE__
#define __HEADER_SQUIRREL_INSTANCE__

#include <vector>
#include <core/util/Variant.hpp>
#include <core/squirrel/ScriptObject.h>

namespace squirrel
{

// TODO Make it inherit ScriptTable?
/// \brief Wraps a Squirrel class instance.
class SN_API Instance : public Object
{
public:
    static bool createRef(HSQUIRRELVM vm, const std::string & fullClassName, HSQOBJECT * out_obj, bool callConstructor=true);
    static bool createNoRef(HSQUIRRELVM vm, const std::string & fullClassName, HSQOBJECT * out_obj, bool callConstructor=true);

    bool create(HSQUIRRELVM vm, const std::string & fullClassName, bool callConstructor=true);

    bool hasMethod(const std::string & methodName);
    bool callMethod(const std::string & methodName);

    void setObject(HSQUIRRELVM vm, HSQOBJECT obj);
    
};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_INSTANCE__

