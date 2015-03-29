#ifndef __HEADER_SN_MATERIAL__
#define __HEADER_SN_MATERIAL__

#include <core/asset/Asset.hpp>
#include <core/math/Vector2.hpp>
#include <core/math/Vector3.hpp>
#include <core/math/Vector4.hpp>

namespace sn
{

class SN_API Material : public Asset
{
public:
    SN_SCRIPT_OBJECT(sn::Material, sn::Asset)

    Material() :
        Asset()
    {}

    //------------------------------------
    // Generic material methods
    //------------------------------------

    // TODO Make them virtual?

    void setParam(const std::string & name, f32 x);
    void setParam(const std::string & name, f32 x, f32 y);
    void setParam(const std::string & name, f32 x, f32 y, f32 z);
    void setParam(const std::string & name, f32 x, f32 y, f32 z, f32 w);
    void setParam(const std::string & name, f32 matrix4x4Values[16]);

protected:
    ~Material() {}

protected:
    std::unordered_map<std::string, f32> m_floats;
    std::unordered_map<std::string, Vector2f> m_vec2;
    std::unordered_map<std::string, Vector3f> m_vec3;
    std::unordered_map<std::string, Vector4f> m_vec4;
    std::unordered_map<std::string, Matrix4> m_mat4;

};

} // namespace sn

#endif // __HEADER_SN_MATERIAL__

