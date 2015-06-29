#include "WindowParams.h"
#include <core/math/Vector2.h>

namespace sn
{

void serialize(JsonBox::Value & o, const WindowParams & params)
{
    // TODO serialize WindowParams
    SN_WARNING("WindowParams serialization not implemented yet");
}

void unserialize(JsonBox::Value & o, WindowParams & params)
{
    if (o.isObject())
    {
        Vector2i size;
        sn::unserialize(o["size"], size, params.rect.size());
        params.rect.size() = size;

        sn::unserialize(o["title"], params.title, params.title);

        JsonBox::Value & styleValues = o["style"];
        if (styleValues.isArray())
        {
            u32 size = styleValues.getArray().size();
            for (u32 i = 0; i < size; ++i)
            {
                const std::string & s = styleValues[i].getString();
                if (s == "fullscreen")
                {
                    params.style |= SN_WS_FULLSCREEN;
                }
                else if (s == "centered")
                {
                    params.style |= SN_WS_CENTERED;
                }
                // TODO support more styles
            }
        }
    }
}

} // namespace sn

