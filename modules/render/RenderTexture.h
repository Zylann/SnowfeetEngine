#ifndef __HEADER_SNR_RENDERTEXTURE__
#define __HEADER_SNR_RENDERTEXTURE__

#include <core/asset/Asset.h>

#include "Texture.h"
//#include "RenderTarget.h"

namespace sn {
namespace render {

class RenderTexture : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::render::RenderTexture, sn::Asset)

    RenderTexture();

    //---------------------------------------
    // RenderTarget interface
    //---------------------------------------

	Vector2u getSize() const;// override;

    //---------------------------------------
    // RenderTexture interface
    //---------------------------------------

    bool create(Vector2u size);
    bool create(Vector2u size, bool hasDepth);

    void destroy();

    Texture * getTexture() const { return m_texture; }

    static void bind(const RenderTexture * rt);

    // TODO

protected:
    ~RenderTexture();

private:
    u32 m_frameBufferID;
    u32 m_depthBufferID;
    bool m_hasDepth;
    Texture * m_texture;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_RENDERTEXTURE__

