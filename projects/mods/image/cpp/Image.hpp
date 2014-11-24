#ifndef __HEADER_SN_IMAGE__
#define __HEADER_SN_IMAGE__

#include <string>
#include <vector>

#include <core/types.hpp>
#include <core/math/Vector2.hpp>
#include <core/math/Color.hpp>
#include <core/asset/Asset.hpp>
#include <core/app/ASScriptObject.hpp>

namespace sn
{

class SN_API Image : public IAsset, public ASScriptObject
{
public:

    //SN_ASSET("image");
    
    Image();

    void create(u32 width, u32 height, Color fillColor = Color());
    void clear();

    bool loadFromFile(const std::string & filePath);
    bool loadFromMemory(const void * data, u32 dataSize);
    bool loadFromPixels(const u8 * pixels, u32 width, u32 height, u32 channels = 4);

    bool saveToFile(const std::string & filePath, std::string formatExtension="");

    inline Vector2u getSize() const { return m_size; }

    inline u32 getPixelIndex(u32 x, u32 y) const
    {
        return (x + y * m_size.x()) * m_channels;
    }

    Color getPixel(u32 x, u32 y) const;
    void setPixel(u32 x, u32 y, Color color);

    void fill(Color color);
private:

    ~Image();

    std::vector<u8> m_pixels;
    Vector2u m_size;
    u32 m_channels;

};

} // namespace sn

#endif // __HEADER_SN_IMAGE__

