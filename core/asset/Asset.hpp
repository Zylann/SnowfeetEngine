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

    /// \deprecated
    /// \brief Tells if the asset pointed by metadata can be loaded by this type.
    /// Common tests include file extension or checking the first bytes of the file.
    virtual bool canLoad(const AssetMetadata & metadata) const { return false; }

    /// \deprecated
    /// \brief Loads the asset from a file (info is in metadata)
    virtual bool loadFromStream(std::ifstream & ifs) { return false; }

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

