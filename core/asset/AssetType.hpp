/*
AssetType.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSET_TYPE__
#define __HEADER_SN_ASSET_TYPE__

#include <core/asset/Asset.hpp>
#include <core/asset/AssetMetadata.hpp>

namespace sn
{

/// \brief Handles the identification of an asset
class SN_API IAssetType
{
public:
    virtual ~IAssetType() {}

    /// \brief Asks if the type matches the given file.
    virtual bool canLoad(String path, const AssetMetadata & meta) const = 0;

	/// \brief Creates an instance of asset of the related type.
	virtual Asset * createInstance() = 0;

	//virtual const char * getDatabaseTypeName() const = 0;

};

} // namespace sn

#endif // __HEADER_SN_ASSET_TYPE__


