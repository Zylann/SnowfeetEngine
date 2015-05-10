#ifndef __HEADER_SNR_TEXTURE__
#define __HEADER_SNR_TEXTURE__

#include <core/asset/base/TextureBase.hpp>
#include <core/math/Vector2.hpp>
#include <GL/glew.h>

namespace sn {
namespace render {

// TODO Make copyable (needs to download pixels?)
class Texture : public TextureBase, public NonCopyable
{
public:
    SN_SCRIPT_OBJECT(sn::render::Texture, sn::TextureBase)

    Texture();

    //---------------------------------------
    // Texture interface
    //---------------------------------------

    void destroy();

    bool create(Vector2u size);
    bool loadFromPixelsRGBA8(Vector2u size, const char * data);

    Vector2u getSize() const { return m_size; }
    GLuint getInternalID() const { return reinterpret_cast<GLuint>(getHandle()); }

    void setSmooth(bool enable);
    void setRepeated(bool enable);

    bool isSmooth() const { return m_isSmooth; }
    bool isRepeated() const { return m_isRepeated; }

    //Texture & operator=(const Texture & other);

    static void bind(Texture * tex);
    static void setActive(u32 textureUnit, Texture * tex);

protected:
    //---------------------------------------
    // TextureBase overrides
    //---------------------------------------

    bool onUploadToVRAM() override;
    bool onDownloadFromVRAM() override;

private:
    ~Texture(); // use release();

    void updateFilter();
    void updateRepeat();

private:
    bool m_isSmooth;
    bool m_isRepeated;

    //GLuint m_samplerID;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_TEXTURE__

