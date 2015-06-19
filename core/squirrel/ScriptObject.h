#ifndef __HEADER_SN_SCRIPTOBJECT__
#define __HEADER_SN_SCRIPTOBJECT__

#include <core/squirrel/bind_tools.hpp>
#include <core/util/NonCopyable.hpp>

namespace sn
{

/// \brief Base class holding a reference to a Squirrel object.
/// C++ takes shared ownership on this instance.
class SN_API ScriptObject
{
public:
    ScriptObject(HSQUIRRELVM vm=nullptr);
    ScriptObject(HSQUIRRELVM vm, HSQOBJECT obj);
    ScriptObject(const ScriptObject & other);

    ~ScriptObject();

    HSQOBJECT getObject() const { return m_object; }
    HSQUIRRELVM getVM() const { return m_vm; }

    bool isNull() const;
    void releaseObject();

    ScriptObject & operator=(const ScriptObject & other);

protected:
    HSQOBJECT m_object;
    HSQUIRRELVM m_vm;
};

} // namespace sn

#endif // __HEADER_SN_SCRIPTOBJECT__

