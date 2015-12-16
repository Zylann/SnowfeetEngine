#ifndef __HEADER_SN_VARIANT_SQUIRREL__
#define __HEADER_SN_VARIANT_SQUIRREL__

#include <squirrel.h>
#include <core/util/Variant.h>

namespace sn
{

/// \brief Pushes a variant on the stack as its Squirrel equivalent
/// \param vm: target Squirrel VM
/// \param o: Source value
void SN_API pushValue(HSQUIRRELVM vm, const Variant & v);
void SN_API pushTable(HSQUIRRELVM vm, const Variant::Dictionary & d);
void SN_API pushArray(HSQUIRRELVM vm, const Variant::Array & a);

/// \brief Walks through a variant dictionary and applies its values to a Squirrel objects at the same paths.
/// \warning Existing sub-values are overwritten.
/// \param vm: target Squirrel VM
/// \param o: Source value
/// \param objectIndex: stack index of the target Squirrel object
void SN_API applyProperties(HSQUIRRELVM vm, const Variant::Dictionary & o, SQInteger objectIndex);

// TODO applyProperties

void SN_API getVariant(HSQUIRRELVM vm, Variant & out_v, SQInteger index);
void SN_API getArray(HSQUIRRELVM vm, Variant & out_v, SQInteger index);
void SN_API getTable(HSQUIRRELVM vm, Variant & out_v, SQInteger index);

} // namespace sn

#endif // __HEADER_SN_VARIANT_SQUIRREL__

