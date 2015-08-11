#ifndef __HEADER_SN_DUMPTOOBJECTDBCONVERTER__
#define __HEADER_SN_DUMPTOOBJECTDBCONVERTER__

#include <core/util/Variant.h>

namespace sn
{

/// \brief Converts old, tree-style JSON dump format into ObjectDB format.
class DumpToObjectDBConverter
{
public:
    /// \brief Converts input JSON. Last output is cleared.
    /// \param input: dump data. Won't be modified (not const for convenience because of JsonBox's design).
    void convert(const Variant & input);

    /// \brief The result of the conversion will be stored here.
    Variant output;

private:
    u32 makeID();
    void extractChildren(Variant & inout_obj, Variant::Array & out_objectsArray);

private:
    u32 m_nextID;
};

} // namespace sn

#endif // __HEADER_SN_DUMPTOOBJECTDBCONVERTER__

