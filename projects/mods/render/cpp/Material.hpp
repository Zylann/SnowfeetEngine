#ifndef __HEADER_SN_RENDER_MATERIAL__
#define __HEADER_SN_RENDER_MATERIAL__

#include <core/asset/Asset.hpp>
#include <core/util/SharedRef.hpp>

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
        m_shader(nullptr)
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

    void apply();

private:
    ~Material();

private:
    SharedRef<ShaderProgram> m_shader;

    std::unordered_map<std::string, SharedRef<Texture> > m_textures;

    bool m_depthTest;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_MATERIAL__

