/*
Asset.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_ASSET__
#define __HEADER_SN_ASSET__

#include <core/config.hpp>
#include <core/util/RefCounted.hpp>

// Put this macro into your asset classes to define its database type name.
// Examples: "image", "texture", "sound", "soundbuffer", "shader", "font"...
#define SN_ASSET(dbTypeName)\
    static const char * __sGetDatabaseTypeName() {\
        static const char * name = dbTypeName; \
        return name; \
    }

namespace sn
{

// TODO Use shared_ptr for resources

/// \brief Interface inherited by all asset classes.
/// Modules are free to register their own types if they derive from IAssetType.
/// If the object you want to turn into an asset is a class already, you have to wrap it.
class SN_API Asset : public RefCounted
{
public:
	Asset() : RefCounted() {}

	virtual bool loadFromFile(const std::string & filePath) = 0;

protected:
	// Because refcount
	// TODO Notify AssetDatabase?
	virtual ~Asset() {}
};

} // namespace sn

#endif // __HEADER_SN_ASSET__

