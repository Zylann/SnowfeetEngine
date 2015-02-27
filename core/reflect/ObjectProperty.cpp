#include "ObjectProperty.hpp"

namespace sn
{

std::string ObjectProperty::toString() const
{
    std::string s = "{" + m_name;
    if (hasGetter())
        s += " get";
    if (hasSetter())
        s += " set";
    return s + "}";
}

} // namespace sn


