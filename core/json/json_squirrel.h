#ifndef __HEADER_SN_JSON_SQUIRREL__
#define __HEADER_SN_JSON_SQUIRREL__

#include <JsonBox.h>
#include <squirrel.h>
#include <core/export.h>

namespace sn
{

/// \brief Pushes a JSON value on the stack as its Squirrel equivalent
/// \param vm: Squirrel VM
/// \param o: Source JSON value
void SN_API pushValue(HSQUIRRELVM vm, const JsonBox::Value & v);

/// \brief Walks through a JSON object and applies its values to a Squirrel objects at the same paths
/// \param vm: Squirrel VM
/// \param o: Source JSON object
/// \param objectIndex: stack index of the target Squirrel object
void SN_API applyProperties(HSQUIRRELVM vm, const JsonBox::Value & o, SQInteger objectIndex);

} // namespace sn

#endif //__HEADER_SN_JSON_SQUIRREL__


