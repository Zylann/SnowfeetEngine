#ifndef __HEADER_SN_UPDATELAYER__
#define __HEADER_SN_UPDATELAYER__

#include <core/util/Variant.h>

namespace sn
{

struct UpdateLayer
{
    UpdateLayer() : enabled(true) {}
    UpdateLayer(const std::string & a_name) : name(a_name), enabled(true) {}

    void serialize(Variant & o);
    void unserialize(const Variant & o);

    std::string name;
    bool enabled;

};

} // namespace sn

#endif // __HEADER_SN_UPDATELAYER__

