#ifndef __HEADER_TGUI_DRAWBATCH__
#define __HEADER_TGUI_DRAWBATCH__

#include <core/scene/base/IDrawContext.hpp>
#include <core/asset/base/Mesh.hpp>
#include <core/asset/base/Material.hpp>

#include "Border.hpp"

namespace tgui
{

class DrawBatch
{
public:
    DrawBatch(sn::IDrawContext & dc);
    ~DrawBatch();

    void setMaterial(sn::Material & m);
    void fillRect(const sn::IntRect & r, const sn::IntRect & texRect, sn::Vector2u ts);
    void fillNineSlices(const sn::IntRect & r, const Border & b, const sn::IntRect & texRect, sn::Vector2u ts);
    void flush();

private:
    sn::Mesh * m_mesh;
    sn::IDrawContext & m_dc;

};

} // namespace tgui

#endif // __HEADER_TGUI_DRAWBATCH__

