#ifndef __HEADER_SN_MATERIAL__
#define __HEADER_SN_MATERIAL__

#include <core/asset/Asset.h>
#include <core/asset/base/TextureBase.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Vector4.h>

namespace sn
{

/// \brief Graphics parameters used when drawing elements
class SN_API Material : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::Material, sn::Asset)

    // Conventional uniform names

    static const char* MAIN_TEXTURE;

    Material() : Asset() {}

    //------------------------------------
    // Generic material methods
    //------------------------------------

    void setParam(const std::string & name, f32 x);
    void setParam(const std::string & name, f32 x, f32 y);
    void setParam(const std::string & name, f32 x, f32 y, f32 z);
    void setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w);
    void setParam(const std::string & name, f32 matrix4x4Values[16]);

    void setTexture(const std::string & name, TextureBase * tex);

    TextureBase * getTexture(const std::string & name) const;

protected:
    ~Material() {}

protected:
    std::unordered_map<std::string, f32> m_floats;
    std::unordered_map<std::string, Vector2f> m_vec2;
    std::unordered_map<std::string, Vector3f> m_vec3;
    std::unordered_map<std::string, Vector4f> m_vec4;
    std::unordered_map<std::string, Matrix4> m_mat4;

    std::unordered_map<std::string, SharedRef<TextureBase> > m_textures;

};

} // namespace sn

#endif // __HEADER_SN_MATERIAL__

