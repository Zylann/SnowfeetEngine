#ifndef __HEADER_SN_ASSETLOCATION__
#define __HEADER_SN_ASSETLOCATION__

#include <core/json/json_utils.hpp>

namespace sn
{

struct AssetLocation
{
    static const char NAMESPACE_SEPARATOR;

    AssetLocation() {}
    AssetLocation(const std::string & fullName);

    std::string getFullName();
    void setFullName(const std::string & fullName);

    bool isEmpty() const;

    /// \brief Name of the asset.
    std::string name;
    /// \brief Namespace of the asset. If not specified, it is relative.
    std::string module;
};

void unserialize(JsonBox::Value & o, AssetLocation & loc);
void serialize(JsonBox::Value & o, AssetLocation & loc);

} // namespace sn

#endif // __HEADER_SN_ASSETLOCATION__


