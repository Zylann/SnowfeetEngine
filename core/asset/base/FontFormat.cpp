#include "FontFormat.hpp"
#include <sstream>

namespace sn
{

std::string toString(FontFormat format)
{
    std::stringstream ss;
    ss << "{size: " << format.size;
    if (format.style & FontFormat::STYLE_BOLD)
        ss << " bold";
    if (format.style & FontFormat::STYLE_ITALIC)
        ss << " italic";
    if (format.style == FontFormat::STYLE_REGULAR)
        ss << " regular";
    ss << "}";
    return ss.str();
}

} // namespace sn

