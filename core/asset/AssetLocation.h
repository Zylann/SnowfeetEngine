#ifndef __HEADER_SN_ASSETLOCATION__
#define __HEADER_SN_ASSETLOCATION__

#include <core/sml/variant_serialize.h>

namespace sn
{

/// \brief Represents the namespaced virtual location of an asset.
/// This location is used in applications rather than filesystem paths,
/// unless the 'name' is computed as a path itself.
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
    std::string project;
};

void unserialize(const Variant & o, AssetLocation & loc);
void serialize(Variant & o, AssetLocation & loc);

} // namespace sn

#endif // __HEADER_SN_ASSETLOCATION__


