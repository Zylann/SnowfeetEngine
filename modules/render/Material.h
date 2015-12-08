#ifndef __HEADER_SN_RENDER_MATERIAL__
#define __HEADER_SN_RENDER_MATERIAL__

#include <core/util/SharedRef.h>
#include <core/math/Vector4.h>

#include <modules/render/common.h>

#include "ShaderProgram.h"
#include "RenderTexture.h"
#include "BlendMode.h"

namespace sn {
namespace render {

/// \brief Defines the appearance of objects
class SN_RENDER_API Material : public sn::Asset
{
public:
    SN_SCRIPT_OBJECT(sn::render::Material, sn::Asset)

    // Conventional uniform names
    static const char * MAIN_TEXTURE;
    static const char * MODEL_MATRIX;
    static const char * VIEW_MATRIX;
    static const char * MODEL_VIEW_MATRIX;
    static const char * PROJECTION_MATRIX;
    static const char * NORMAL_MATRIX;
    static const char * COLOR;
    static const char * TIME;

    Material() :
        sn::Asset(),
        m_shader(nullptr),
        m_depthTest(false)
    {}

    template <typename T>
    T getParam(const std::string & name)
    {
        T v;
        getParam(name, v);
        return v;
    }

    bool getParam(const std::string & name, f32 & out_v);
    // TODO Other getters

    void setParam(const std::string & name, f32 x);
    void setParam(const std::string & name, f32 x, f32 y);
    void setParam(const std::string & name, f32 x, f32 y, f32 z);
    void setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w);
    void setParam(const std::string & name, f32 matrix4x4Values[16]);

    void setTexture(const std::string & name, Texture * tex);
    void setRenderTexture(const std::string & name, RenderTexture * tex);
    Texture * getTexture(const std::string & name) const;

    void setShader(ShaderProgram * shader);
    inline ShaderProgram * getShader() const { return m_shader.get(); }

    // Depth buffer requirement
    bool isDepthTest() const { return m_depthTest; }
    void setDepthTest(bool zTest) { m_depthTest = zTest; }

    BlendMode getBlendMode() const { return m_blendMode; }
    void setBlendMode(BlendMode mode) { m_blendMode = mode; }

    void applyParameters();

private:
    ~Material();

private:
    SharedRef<ShaderProgram> m_shader;

    std::unordered_map<std::string, f32> m_floats;
    std::unordered_map<std::string, Vector2f> m_vec2;
    std::unordered_map<std::string, Vector3f> m_vec3;
    std::unordered_map<std::string, Vector4f> m_vec4;
    std::unordered_map<std::string, Matrix4> m_mat4;

    std::unordered_map< std::string, SharedRef<Texture> > m_textures;

    bool m_depthTest;
    BlendMode m_blendMode;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_MATERIAL__

