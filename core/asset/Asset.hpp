#ifndef __HEADER_SN_ASSET__
#define __HEADER_SN_ASSET__

#include <core/config.hpp>
#include <core/app/ASScriptObject.hpp>

// Put this macro into your asset classes to define its database type name.
// Examples: "image", "texture", "sound", "soundbuffer", "shader", "font"...
#define SN_ASSET(dbTypeName)\
    static const char * sGetDatabaseTypeName() {\
        static const char * name = dbTypeName; \
        return name; \
    }

namespace sn
{

/// \brief Interface inherited by all asset classes.
/// Modules are free to register their own types if they derive from IAssetType.
/// If the object you want to turn into an asset is a class already, you have to wrap it.
class SN_API Asset : public ASScriptObject
{
public:
	Asset() : ASScriptObject() {}

	virtual bool loadFromFile(const std::string & filePath) = 0;

protected:
	// Because refcount
	// TODO Notify AssetDatabase?
	virtual ~Asset() {}
};

} // namespace sn

#endif // __HEADER_SN_ASSET__

