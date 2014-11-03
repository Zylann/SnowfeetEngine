#include "Object.hpp"

namespace sn
{

// Static
sn::ObjectType & Object::sObjectType()
{
    static sn::ObjectType t("sn::Object", "");
    return t;
}


} // namespace sn

