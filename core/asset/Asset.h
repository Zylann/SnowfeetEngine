/*
Asset.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSET__
#define __HEADER_SN_ASSET__

#include <core/config.h>
#include <core/app/ScriptableObject.h>
#include <core/asset/AssetMetadata.h>
#include <core/asset/AssetLocation.h>

namespace sn
{

class AssetDatabase;

/// \brief Interface inherited by all asset classes.
/// It is a container which can have loaded or unloaded state, with additional metadata.
/// Modules are free to register their own types if they derive from Asset.
/// Note: If the object you want to turn into an asset is a class already, you have to wrap it.
class SN_API Asset : public ScriptableObject
{
public:
    SN_OBJECT

    /// \brief Default constructor.
    /// All assets have a default constructor making an empty asset.
	Asset() : ScriptableObject() {}

    inline const AssetMetadata & getAssetMetadata() const { return m_metadata; }

protected:
	// Because refcount
	// TODO Notify AssetDatabase?
	virtual ~Asset() {}

private:
    friend class AssetDatabase;

    AssetMetadata m_metadata;
};

} // namespace sn

#endif // __HEADER_SN_ASSET__

