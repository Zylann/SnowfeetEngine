#include "scripttype.h"

#include <string>
#include <assert.h>
#include <new>

BEGIN_AS_NAMESPACE

// Note: generic calling convention was used to have access to the engine,
// so we can get the asIObjectType from the type ID
static void CScriptType_InstanceConstruct(asIScriptGeneric * g)
{
    // Get type of the first argument
    int typeID = g->GetArgTypeId(0);

    asIObjectType * ot = g->GetEngine()->GetObjectTypeById(typeID);

    if (strcmp(ot->GetNamespace(), "") == 0 && strcmp(ot->GetName(), "type") == 0)
    {
        // The argument is a type<T> instance, get the T instead
        ot = ot->GetSubType();
    }

    new(g->GetObject())CScriptType(ot);
}

static void CScriptType_TemplateConstruct(asIObjectType *type, void *mem)          { new(mem)CScriptType(type->GetSubType()); }
//static void CScriptType_GenericDefaultConstruct(asIScriptGeneric * g, void * mem)  { new(mem)CScriptType(NULL); }
static void CScriptType_DefaultConstruct(void * mem)                               { new(mem)CScriptType(NULL); }
static void CScriptType_Destruct(CScriptType *obj)                                 { obj->~CScriptType(); }
static CScriptType CScriptType_TemplateToGeneric(CScriptType * self)               { return *self; }

void RegisterScriptType(asIScriptEngine *engine)
{
    asIScriptEngine & e = *engine;

    std::string c = "Type";
    std::string ct_ = "type";
    std::string ct = ct_ + "<T>";

    //------------------
    // Declare types
    //------------------

    int r = 0;

    // type declarations
    r = e.RegisterObjectType((ct_ + "<class T>").c_str(), sizeof(CScriptType), asOBJ_VALUE | asOBJ_APP_CLASS_DA | asOBJ_TEMPLATE); assert(r >= 0);
    r = e.RegisterObjectType(c.c_str(), sizeof(CScriptType), asOBJ_VALUE | asOBJ_APP_CLASS_DA); assert(r >= 0);

    //------------------
    // type<T>
    //------------------

    r = e.RegisterObjectBehaviour(ct.c_str(), asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(CScriptType_TemplateConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);
    r = e.RegisterObjectBehaviour(ct.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(CScriptType_Destruct), asCALL_CDECL_OBJLAST); assert(r >= 0);

    // Conversion to Type (turns out... they're the same C++ thing)
    r = e.RegisterObjectMethod(ct.c_str(), (c + " opImplConv() const").c_str(), asFUNCTION(CScriptType_TemplateToGeneric), asCALL_CDECL_OBJLAST); assert(r >= 0);

#if AS_ADDON_SCRIPTTYPE_USE_STLSTRING == 1
    r = engine->RegisterObjectMethod(ct.c_str(), "string opImplConv() const", asMETHOD(CScriptType, ToString), asCALL_THISCALL); assert(r >= 0);
#endif

    //------------------
    // Type
    //------------------

    // We are forced to register a default constructor, however it doesn't make any sense here...
    r = e.RegisterObjectBehaviour(c.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(CScriptType_DefaultConstruct), asCALL_CDECL_OBJLAST); assert(r >= 0);

    r = e.RegisterObjectBehaviour(c.c_str(), asBEHAVE_CONSTRUCT, "void f(const ?&in)", asFUNCTION(CScriptType_InstanceConstruct), asCALL_GENERIC); assert(r >= 0);
    r = e.RegisterObjectBehaviour(c.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(CScriptType_Destruct), asCALL_CDECL_OBJLAST); assert(r >= 0);

    r = e.RegisterObjectMethod(c.c_str(), (c + " &opAssign(const " + c + "&in)").c_str(), asMETHOD(CScriptType, operator=), asCALL_THISCALL); assert(r >= 0);
    r = e.RegisterObjectMethod(c.c_str(), ("bool opEquals(const " + c + "&in) const").c_str(), asMETHOD(CScriptType, operator==), asCALL_THISCALL); assert(r >= 0);
    r = e.RegisterObjectMethod(c.c_str(), ("bool derivesFrom(const " + c + "&in) const").c_str(), asMETHOD(CScriptType, DerivesFrom), asCALL_THISCALL); assert(r >= 0);

#if AS_ADDON_SCRIPTTYPE_USE_STLSTRING == 1
    r = engine->RegisterObjectMethod(c.c_str(), "string opImplConv() const", asMETHOD(CScriptType, ToString), asCALL_THISCALL); assert(r >= 0);
#endif

}


END_AS_NAMESPACE

