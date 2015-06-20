#ifndef __HEADER_SQUIRREL_OBJECT__
#define __HEADER_SQUIRREL_OBJECT__

#include <core/squirrel/bind_tools.hpp>
#include <core/util/NonCopyable.hpp>

namespace squirrel
{

/// \brief Base class holding a reference to a Squirrel object.
/// C++ takes shared ownership on this instance.
class SN_API Object
{
public:
    Object(HSQUIRRELVM vm=nullptr);
    Object(HSQUIRRELVM vm, HSQOBJECT obj);
    Object(const Object & other);

    ~Object();

    HSQOBJECT getObject() const { return m_object; }
    HSQUIRRELVM getVM() const { return m_vm; }

    bool isNull() const;
    void releaseObject();

    Object & operator=(const Object & other);

protected:
    HSQOBJECT m_object;
    HSQUIRRELVM m_vm;
};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_OBJECT__

