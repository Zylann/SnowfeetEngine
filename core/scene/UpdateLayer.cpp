#include "UpdateLayer.h"
#include <core/sml/variant_serialize.h>

namespace sn
{

void UpdateLayer::serialize(Variant & o)
{
    sn::serialize(o["name"], name);
    sn::serialize(o["enabled"], enabled);
}

void UpdateLayer::unserialize(const Variant & o)
{
    sn::unserialize(o["name"], name);
    sn::unserialize(o["enabled"], enabled, true);
}

} // namespace sn

