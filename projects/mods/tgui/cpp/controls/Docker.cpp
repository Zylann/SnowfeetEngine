#include "Docker.h"

using namespace sn;

namespace tgui
{

void Docker::layoutChildren()
{
    Vector2i size = getSize();
    m_split.layout(IntRect::fromPositionSize(0,0,size.x(), size.y()));
}

void Docker::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    m_split.serializeTree(o, ctx);
}

void Docker::unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

    // TODO Controls are referenced by child index, it should be done with ObjectDB references

    m_split.unserializeTree(*this, o["split"], ctx);

}


} // namespace tgui

