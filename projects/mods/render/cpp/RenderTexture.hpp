#ifndef __HEADER_SNR_RENDERTEXTURE__
#define __HEADER_SNR_RENDERTEXTURE__

#include <core/asset/Asset.h>

#include "Texture.hpp"
//#include "RenderTarget.hpp"

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
    GLuint m_frameBufferID;
    GLuint m_depthBufferID;
    bool m_hasDepth;
    Texture * m_texture;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_RENDERTEXTURE__

