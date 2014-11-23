#include "AbstractTransform.hpp"

namespace sn
{

SN_OBJECT_IMPL(sn::AbstractTransform)

//------------------------------------------------------------------------------
void AbstractTransform::translate(sn::Vector3f motion)
{
    setPosition(position() + motion);
}

//------------------------------------------------------------------------------
//void AbstractTransform::rotate(f32 degrees)
//{
//	setRotation(rotation() + degrees);
//}

//------------------------------------------------------------------------------
void AbstractTransform::scale(sn::Vector3f scaleFactor)
{
    sn::Vector3f oldScale = scale();
    setScale(oldScale * scaleFactor);
}

} // namespace zn



