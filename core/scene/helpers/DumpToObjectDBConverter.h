#ifndef __HEADER_SN_DUMPTOOBJECTDBCONVERTER__
#define __HEADER_SN_DUMPTOOBJECTDBCONVERTER__

#include <core/json/json_utils.h>

namespace sn
{

/// \brief Converts old, tree-style JSON dump format into ObjectDB format.
class DumpToObjectDBConverter
{
public:
    /// \brief Converts input JSON. Last output is cleared.
    /// \param input: dump data. Won't be modified (not const for convenience because of JsonBox's design).
    void convert(JsonBox::Value & input);

    /// \brief The result of the conversion will be stored here.
    JsonBox::Value output;

private:
    u32 makeID();
    void extractChildren(JsonBox::Value & obj, JsonBox::Array & objectsArray);

private:
    u32 m_nextID;
};

} // namespace sn

#endif // __HEADER_SN_DUMPTOOBJECTDBCONVERTER__

