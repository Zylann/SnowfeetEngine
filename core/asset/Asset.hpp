/*
Asset.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSET__
#define __HEADER_SN_ASSET__

#include <core/config.hpp>
#include <core/app/ScriptObject.hpp>
#include <core/asset/AssetMetadata.hpp>
#include <core/asset/AssetLocation.hpp>

namespace sn
{

class AssetDatabase;

/// \brief Interface inherited by all asset classes.
/// It is a container which can have loaded or unloaded state, with additional metadata.
/// Modules are free to register their own types if they derive from Asset.
/// Note: If the object you want to turn into an asset is a class already, you have to wrap it.
class SN_API Asset : public ScriptObject
{
public:
    SN_SCRIPT_OBJECT(sn::Asset, sn::ScriptObject)

    /// \brief Default constructor.
    /// All assets have a default constructor making an empty asset.
	Asset() : ScriptObject() {}

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

