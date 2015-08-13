#include "WindowParams.h"
#include <core/math/Vector2.h>

namespace sn
{

void serialize(Variant & o, const WindowParams & params)
{
    // TODO serialize WindowParams
    SN_WARNING("WindowParams serialization not implemented yet");
}

void unserialize(const Variant & o, WindowParams & params)
{
    if (o.isDictionary())
    {
        Vector2i size;
        sn::unserialize(o["size"], size, params.rect.size());
        params.rect.size() = size;

        sn::unserialize(o["title"], params.title, params.title);

        const Variant & styleValues = o["style"];
        if (styleValues.isArray())
        {
            const Variant::Array & styleArray = styleValues.getArray();
            for (u32 i = 0; i < styleArray.size(); ++i)
            {
                const std::string & s = styleArray[i].getString();
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

