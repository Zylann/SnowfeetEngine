#ifndef __HEADER_SN_ASSET__
#define __HEADER_SN_ASSET__

#include <core/config.hpp>

// Put this macro into your asset classes to define its database type name.
// Examples: "image", "texture", "sound", "soundbuffer", "shader", "font"...
#define SN_ASSET(dbTypeName)\
    static const char * sGetDatabaseTypeName() {\
        static const char * name = dbTypeName; \
        return name; \
    }

namespace sn
{

/// \brief Base class of all assets.
/// Modules are free to register their own types as long as they provide loaders.
/// If the object you want to turn into an asset is a class already, you have to wrap it.
class SN_API IAsset
{
public:
    virtual ~IAsset() {}
};

} // namespace sn

#endif // __HEADER_SN_ASSET__

