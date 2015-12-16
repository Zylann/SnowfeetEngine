#ifndef __HEADER_SNR_TEXTURE__
#define __HEADER_SNR_TEXTURE__

#include <core/math/Vector2.h>
#include <core/util/SharedRef.h>

#include <modules/image/Image.h>
#include <modules/render/common.h>

namespace sn
{

typedef void* TextureHandle;

// TODO Make copyable (needs to download pixels?)
class SN_RENDER_API Texture : public sn::Asset, public NonCopyable
{
public:
    SN_OBJECT

    Texture();

    //---------------------------------------
    // Texture interface
    //---------------------------------------

    bool uploadToVRAM();
    bool downloadFromVRAM();

    /// \brief Gets the pixels from the texture currently stored in memory.
    /// If the pixels aren't stored or are not up to date with VRAM,
    /// you should call downloadFromVRAM() instead.
    Image * getImage() const;

    /// \brief Sets the source image for this texture.
    /// A reference will be added to the Image object.
    /// To apply the change, you should call uploadToVRAM() afterwards.
    void setSourceImage(Image & img);

    /// \brief Gets the last known size of the texture.
    Vector2u getSize();

    /// \brief Gets the implementation-specific texture handle.
    /// Example: OpenGL = GLuint, D3D11 might be ID3D11Texture2D* etc.
    TextureHandle getHandle() const { return m_handle; }

    /// \brief Sets if source image's data should be kept in memory after upload.
    void setKeepSourceInMemory(bool enable) { m_keepSourceInMemory = enable; }

    /// \brief Tells if the source image's data should be kept in memory
    bool isKeepSourceInMemory() const { return m_keepSourceInMemory; }

    void destroy();

    bool create(Vector2u size);
    bool loadFromPixelsRGBA8(Vector2u size, const char * data);

    void setSmooth(bool enable);
    void setRepeated(bool enable);

    bool isSmooth() const { return m_isSmooth; }
    bool isRepeated() const { return m_isRepeated; }

    //Texture & operator=(const Texture & other);

    static void bind(Texture * tex);
    static void setActive(u32 textureUnit, Texture * tex);

private:
    ~Texture(); // use release();

    void updateFilter();
    void updateRepeat();

private:
    bool m_isSmooth;
    bool m_isRepeated;
    bool m_keepSourceInMemory;

    /// \brief Reference to the source image.
    /// Can be null if the texture has been created from VRAM and has not been downloaded yet.
    SharedRef<Image> m_image;

    /// \brief Size of the highest mip-map level of the texture.
    /// It is stored here because the source image can be unloaded after upload to VRAM.
    Vector2u m_size;

    /// \brief Implementation-specific handle to the texture object.
    /// In OpenGL this is a GLuint, in D3D11 it would be an ID3D11Texture2D.
    TextureHandle m_handle;

};

} // namespace sn

#endif // __HEADER_SNR_TEXTURE__

