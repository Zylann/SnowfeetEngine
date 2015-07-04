#ifndef __HEADER_SQUIRREL_INSTANCE__
#define __HEADER_SQUIRREL_INSTANCE__

#include <vector>
#include <core/util/Variant.h>
#include <core/squirrel/Object.h>
#include <core/squirrel/push.h>

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

    bool setMember(const char * name, HSQOBJECT obj);
    bool setMemberNull(const char * name);

    template <typename T>
    bool callMethod(const std::string & methodName, T arg)
    {
        if (!getMemberOnStack(methodName))
            return false;

        // Push this
        sq_pushobject(m_vm, m_object);

        // Push argument
        squirrel::push(m_vm, arg);

        // Call the method
        if (SQ_FAILED(sq_call(m_vm, 2, SQFalse, SQTrue)))
        {
            sq_pop(m_vm, 2);
            SN_ERROR("Squirrel error on method call");
            return false;
        }

        // Pop the function and the roottable
        sq_pop(m_vm, 2);

        return true;
    }

private:
    bool getMemberOnStack(const std::string & key);

};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_INSTANCE__

