#ifndef __HEADER_SN_TEXTUREBASE__
#define __HEADER_SN_TEXTUREBASE__

#include <core/util/SharedRef.h>
#include <core/asset/base/Image.h>

namespace sn
{

typedef void* TextureHandle;

/// \brief Common asset for images loaded into VRAM for accelerated rendering.
class SN_API TextureBase : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::TextureBase, sn::Asset)

    TextureBase() : Asset(),
        m_handle(nullptr),
        m_keepSourceInMemory(false)
    {}

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
    void setKeepSourceInMemory(bool enable) { m_keepSourceInMemory = true; }

    /// \brief Tells if the source image's data should be kept in memory
    bool isKeepSourceInMemory() const { return m_keepSourceInMemory; }

protected:
    virtual bool onUploadToVRAM() = 0;
    virtual bool onDownloadFromVRAM() = 0;

protected:
    /// \brief Reference to the source image.
    /// Can be null if the texture has been created from VRAM and has not been downloaded yet.
    SharedRef<Image> m_image;

    /// \brief Size of the highest mip-map level of the texture.
    /// It is stored here because the source image can be unloaded after upload to VRAM.
    Vector2u m_size;

    /// \brief Implementation-specific handle to the texture object.
    /// In OpenGL this is a GLuint, in D3D11 it would be an ID3D11Texture2D.
    void * m_handle;

private:
    bool m_keepSourceInMemory;

};

} // namespace sn

#endif // __HEADER_SN_TEXTUREBASE__


