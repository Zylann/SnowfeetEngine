#include "AssetLocation.h"

namespace sn
{

const char AssetLocation::NAMESPACE_SEPARATOR = ':';

AssetLocation::AssetLocation(const std::string & fullName)
{
    setFullName(fullName);
}

std::string AssetLocation::getFullName()
{
    if (project.empty())
        return name;
    else
        return project + NAMESPACE_SEPARATOR + name;
}

void AssetLocation::setFullName(const std::string & fullName)
{
    size_t i = fullName.find(NAMESPACE_SEPARATOR);
    if (i != std::string::npos)
    {
        project = fullName.substr(0, i);
        name = fullName.substr(i+1);
    }
    else
    {
        name = fullName;
    }
}

bool AssetLocation::isEmpty() const
{
    return name.empty();
}

void unserialize(const Variant & o, AssetLocation & loc)
{
    if (o.isString())
    {
        loc.setFullName(o.getString());
    }
}

void serialize(Variant & o, AssetLocation & loc)
{
    o = loc.getFullName();
}

} // namespace sn

