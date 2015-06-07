#ifndef __HEADER_SN_SCRIPT__
#define __HEADER_SN_SCRIPT__

#include <core/squirrel/ScriptObject.h>
#include <string>

namespace sn
{

/// \brief Wraps a script closure that can be executed.
class SN_API Script : public ScriptObject
{
public:
    Script(HSQUIRRELVM vm);

    bool compileString(
        const std::string & sourceCode, 
        const std::string & scriptName = _SC("(unnamed script)")
    );

    bool execute();

};

} // namespace sn

#endif // __HEADER_SN_SCRIPT__

