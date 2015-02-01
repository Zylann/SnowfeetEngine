#ifndef __HEADER_SN_SCRIPTOBJECT__
#define __HEADER_SN_SCRIPTOBJECT__

#include <core/squirrel/sqrat.h>
#include <core/reflect/Object.hpp>
#include <core/util/RefCounted.hpp>

#define _SN_DECLARE_PUSHSQUIRRELOBJ(_className)                        \
    virtual void pushSquirrelObject(HSQUIRRELVM vm) {                  \
        Sqrat::ClassType<_className>::PushInstance(vm, this);          \
        addRef();                                                      \
    }

#define SN_SCRIPT_OBJECT(_className, _baseName)                        \
    _SN_DECLARE_PUSHSQUIRRELOBJ(_className)                            \
    SN_OBJECT(_className, _baseName)

namespace sn
{

/// \brief All classes accessible and creatable from scripts must inherit this class.
class SN_API ScriptObject : public RefCounted, public Object
{
public:
    SN_SCRIPT_OBJECT(sn::ScriptObject, sn::Object)
};

} // namespace sn

#endif // __HEADER_SN_SCRIPTOBJECT__

