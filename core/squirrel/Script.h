#ifndef __HEADER_SQUIRREL_SCRIPT__
#define __HEADER_SQUIRREL_SCRIPT__

#include <core/squirrel/Object.h>
#include <string>

namespace squirrel
{

/// \brief Wraps a script closure that can be executed.
class SN_API Script : public Object
{
public:
    Script(HSQUIRRELVM vm);

    bool compileString(
        const std::string & sourceCode, 
        const std::string & scriptName = _SC("(unnamed script)")
    );

    bool execute();

};

} // namespace squirrel

#endif // __HEADER_SQUIRREL_SCRIPT__

