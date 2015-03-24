#ifndef __HEADER_SNR_TEXTURE__
#define __HEADER_SNR_TEXTURE__

#include <core/asset/Asset.hpp>
#include <core/math/Vector2.hpp>
#include <GL/glew.h>

namespace sn {
namespace render {

// TODO Make copyable (needs to download pixels?)
class Texture : public Asset, public NonCopyable
{
public:
    SN_SCRIPT_OBJECT(sn::render::Texture, sn::Asset)

    Texture();

    //---------------------------------------
    // Asset interface
    //---------------------------------------

    bool canLoad(const AssetMetadata & meta) const override;
    bool loadFromStream(std::ifstream & ifs) override;

    //---------------------------------------
    // Texture interface
    //---------------------------------------

    void destroy();

    bool create(Vector2u size);
    bool loadFromPixelsRGBA8(Vector2u size, const char * data);

    Vector2u getSize() const { return m_size; }

    //Texture & operator=(const Texture & other);

    static void bind(Texture * tex);

private:
    ~Texture(); // use release();

private:
    Vector2u m_size;
    GLuint m_textureID;
    bool m_isSmooth;
    bool m_isRepeated;

    //GLuint m_samplerID;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_TEXTURE__

