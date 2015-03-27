#ifndef __HEADER_SN_RENDER_MATERIAL__
#define __HEADER_SN_RENDER_MATERIAL__

#include <core/asset/Asset.hpp>
#include <core/util/SharedRef.hpp>
#include <core/math/Vector4.hpp>

#include "ShaderProgram.hpp"
#include "RenderTexture.hpp"

namespace sn {
namespace render {

class Material : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::render::Material, sn::Asset)

    Material() :
        Asset(),
        m_shader(nullptr),
        m_depthTest(false)
    {}

    //------------------------------------
    // Asset interface
    //------------------------------------

    bool canLoad(const AssetMetadata & meta) const override;
    bool loadFromStream(std::ifstream & ifs) override;

    //------------------------------------
    // Methods
    //------------------------------------

    void setShader(ShaderProgram * shader);
    inline ShaderProgram * getShader() const { return m_shader.get(); }

    // Depth buffer requirement
    bool isDepthTest() const { return m_depthTest; }
    void setDepthTest(bool zTest) { m_depthTest = zTest; }

    void setParam(const std::string & name, Texture * tex);
    void setParam(const std::string & name, RenderTexture * tex);
    void setParam(const std::string & name, f32 x);
    void setParam(const std::string & name, f32 x, f32 y);
    void setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w);

    void apply();

private:
    ~Material();

private:
    SharedRef<ShaderProgram> m_shader;

    std::unordered_map<std::string, SharedRef<Texture> > m_textures;
    std::unordered_map<std::string, f32> m_floats;
    std::unordered_map<std::string, Vector2f> m_vec2;
    std::unordered_map<std::string, Vector4f> m_vec4;

    bool m_depthTest;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_MATERIAL__

