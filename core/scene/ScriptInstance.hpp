#ifndef __HEADER_SN_SCRIPTINSTANCE__
#define __HEADER_SN_SCRIPTINSTANCE__

#include <vector>
#include <core/util/Variant.hpp>
#include <squirrel.h>

namespace sn
{

/// \brief Handles script class instance.
/// C++ has ownership on this instance.
class SN_API ScriptInstance
{
public:
    ScriptInstance();
    ~ScriptInstance();

    bool create(const std::string & name);
    bool destroy();

    bool isNull() const;

    Variant getProperty(const std::string & name);

    bool hasMethod(const std::string & name);

    bool callMethod(const std::string & methodName);

private:
    HSQUIRRELVM getVM() const;

private:
    HSQOBJECT m_sqObject;
    
};

} // namespace sn

#endif // __HEADER_SN_SCRIPTINSTANCE__

