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

void serialize(JsonBox::Value & o, const FontFormat & format)
{
    sn::serialize(o["size"], format.size);

    if (format.isBold())
        o["bold"] = true;
    if (format.isItalic())
        o["italic"] = true;
}

void unserialize(JsonBox::Value & o, FontFormat & format)
{
    sn::unserialize(o["size"], format.size, format.size);
    
    if (o["bold"].getBoolean())
        format.style |= FontFormat::STYLE_BOLD;
    if (o["italic"].getBoolean())
        format.style |= FontFormat::STYLE_ITALIC;
}

} // namespace sn

