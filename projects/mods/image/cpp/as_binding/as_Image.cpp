#include <core/app/ScriptEngine.hpp>
#include "as_image.hpp"
#include "../Image.hpp"

namespace sn
{

static void Image_defaultConstructor(Image *self)
{
    new(self)Image();
}

static bool Image_saveToFile(const std::string & path, Image *self)
{
    return self->saveToFile(path);
}

inline bool checkCoordinates(u32 x, u32 y, Image * img, const char * context)
{
    if (x < img->getSize().x() && y < img->getSize().y())
    {
        return true;
    }
    else
    {
        SN_ERROR("Image::" << context << ": wrong coordinates (" << x << "," << y << "), image size is " << toString(img->getSize()));
        return false;
    }
}

static Color Image_getPixel(u32 x, u32 y, Image *self)
{
    if (checkCoordinates(x,y,self, "getPixel"))
    {
        return self->getPixel(x, y);
    }
    else
    {
        return Color();
    }
}

static void Image_setPixel(u32 x, u32 y, Color c, Image *self)
{
    if (checkCoordinates(x,y, self, "setPixel"))
    {
        self->setPixel(x, y, c);
    }
}

static s32 Image_addRef(Image * self) { return self->addRef(); }
static s32 Image_release(Image * self) { return self->release(); }
static asILockableSharedBool *Image_getWeakRefFlag(Image * self) { return self->getWeakRefFlag(); }

void register_image(asIScriptEngine & e)
{
    std::string t = "Image";

    // Type declaration (scripts can create the Scene)
    asCheck(e.RegisterObjectType(t.c_str(), 0, asOBJ_REF | asOBJ_GC));

    // Constructor
    asCheck(e.RegisterObjectBehaviour(t.c_str(), asBEHAVE_CONSTRUCT, (t + "@ f()").c_str(), asFUNCTION(Image_defaultConstructor), asCALL_CDECL_OBJLAST));

    // Refcount
    asCheck(e.RegisterObjectBehaviour(t.c_str(), asBEHAVE_ADDREF, "void f()", asFUNCTION(Image_addRef), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t.c_str(), asBEHAVE_RELEASE, "void f()", asFUNCTION(Image_release), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectBehaviour(t.c_str(), asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asFUNCTION(Image_getWeakRefFlag), asCALL_CDECL_OBJLAST));

    // Methods
    asCheck(e.RegisterObjectMethod(t.c_str(), "bool loadFromFile(const string &in path)", asMETHOD(Image, loadFromFile), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(t.c_str(), "bool saveToFile(const string &in path)", asFUNCTION(Image_saveToFile), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t.c_str(), "Color getPixel(uint x, uint y)", asFUNCTION(Image_getPixel), asCALL_CDECL_OBJLAST));
    asCheck(e.RegisterObjectMethod(t.c_str(), "void setPixel(uint x, uint y, Color c)", asFUNCTION(Image_setPixel), asCALL_CDECL_OBJLAST));

}

} // namespace sn

