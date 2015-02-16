#ifndef __HEADER_SN_RENDER_MATERIAL__
#define __HEADER_SN_RENDER_MATERIAL__

#include <core/asset/Asset.hpp>
#include "ShaderProgram.hpp"

namespace sn {
namespace render {

class Material : public Asset
{
public:
    SN_ASSET(sn::render::Material)

    Material() :
        Asset(),
        m_shader(nullptr)
    {}

    //------------------------------------
    // Asset interface
    //------------------------------------

    bool canLoad(const AssetMetadata & meta) const override;
    // TODO loadFromStream()

    //------------------------------------
    // Methods
    //------------------------------------

    void setShader(ShaderProgram * shader);
    inline ShaderProgram * getShader() const { return m_shader; }

private:
    ~Material();

private:
    ShaderProgram * m_shader;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDER_MATERIAL__

