#include <core/util/stringutils.hpp>
#include <core/json/json_utils.hpp>

#include "RenderTexture.hpp"
#include "gl_check.hpp"

namespace sn {
namespace render {

RenderTexture::RenderTexture(): Asset(),
    m_frameBufferID(0),
    m_depthBufferID(0),
    m_texture(nullptr),
    m_hasDepth(true)
{
    m_texture = new Texture();
}

RenderTexture::~RenderTexture()
{
    destroy();
}

bool RenderTexture::create(Vector2u size)
{
    return create(size, m_hasDepth);
}

bool RenderTexture::create(Vector2u size, bool hasDepth)
{
    SN_ASSERT(m_texture != nullptr, "Invalid state");

    if (!m_texture->create(size))
    {
        return false;
    }

    // Create framebuffer
    if (m_frameBufferID == 0)
    {
        glCheck(glGenFramebuffersEXT(1, &m_frameBufferID));
        if (m_frameBufferID == 0)
        {
            SN_ERROR("Couldn't create framebuffer");
            return false;
        }
    }

    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID));

    // Attach color buffer from the texture to the framebuffer
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->getInternalID(), 0));

    m_hasDepth = hasDepth;

    // If we need a depth buffer
    if (m_hasDepth)
    {
        if (m_depthBufferID == 0)
        {
            // Create depth buffer
            glCheck(glGenRenderbuffers(1, &m_depthBufferID));
            if (m_depthBufferID != 0)
            {
                glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID));

                // Initialize the depth buffer
                glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.x(), size.y()));

                // Attach depth buffer to the framebuffer
                glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID));
            }
            else
            {
                SN_ERROR("Couldn't create depth buffer");
            }
        }
    }

    // Check buffers linking
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        SN_ERROR("Failed to link render texture buffer");
        return false;
    }

    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    return true;
}

void RenderTexture::destroy()
{
    if (m_frameBufferID)
    {
        glCheck(glDeleteFramebuffers(1, &m_frameBufferID));
        m_frameBufferID = 0;
    }
    if (m_depthBufferID)
    {
        glCheck(glDeleteRenderbuffers(1, &m_depthBufferID));
        m_depthBufferID = 0;
    }
    if (m_texture)
    {
        m_texture->release();
        m_texture = nullptr;
    }
}

Vector2u RenderTexture::getSize() const
{
    if (m_texture)
        return m_texture->getSize();
    else
        return Vector2u(0, 0);
}

void RenderTexture::bind(const RenderTexture * rt)
{
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, rt ? rt->m_frameBufferID : 0));
}

} // namespace render
} // namespace sn

