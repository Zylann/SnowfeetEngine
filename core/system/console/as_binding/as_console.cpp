#include "../console.hpp"
#include "../../../app/ScriptEngine.hpp"

namespace sn
{

//------------------------------------------------------------------------------
void register_ConsoleColor(asIScriptEngine & e)
{
	const char * t = "ConsoleColor";

	asCheck(e.RegisterEnum(t));

	asCheck(e.RegisterEnumValue(t, "BLACK", SN_CC_BLACK));
	asCheck(e.RegisterEnumValue(t, "DARKGRAY", SN_CC_DARKGRAY));
	asCheck(e.RegisterEnumValue(t, "GRAY", SN_CC_GRAY));
	asCheck(e.RegisterEnumValue(t, "WHITE", SN_CC_WHITE));

	asCheck(e.RegisterEnumValue(t, "DARK_RED", SN_CC_DARK_RED));
	asCheck(e.RegisterEnumValue(t, "DARK_YELLOW", SN_CC_DARK_YELLOW));
	asCheck(e.RegisterEnumValue(t, "DARK_GREEN", SN_CC_DARK_GREEN));
	asCheck(e.RegisterEnumValue(t, "DARK_BLUE", SN_CC_DARK_BLUE));
	asCheck(e.RegisterEnumValue(t, "DARK_CYAN", SN_CC_DARK_CYAN));
	asCheck(e.RegisterEnumValue(t, "DARK_MAGENTA", SN_CC_DARK_MAGENTA));

	asCheck(e.RegisterEnumValue(t, "RED", SN_CC_RED));
	asCheck(e.RegisterEnumValue(t, "YELLOW", SN_CC_YELLOW));
	asCheck(e.RegisterEnumValue(t, "GREEN", SN_CC_GREEN));
	asCheck(e.RegisterEnumValue(t, "BLUE", SN_CC_BLUE));
	asCheck(e.RegisterEnumValue(t, "CYAN", SN_CC_CYAN));
	asCheck(e.RegisterEnumValue(t, "MAGENTA", SN_CC_MAGENTA));

	asCheck(e.RegisterEnumValue(t, "DEFAULT", SN_CC_DEFAULT));
}

//------------------------------------------------------------------------------
void register_console(asIScriptEngine & e)
{
	e.SetDefaultNamespace("console");

	register_ConsoleColor(e);

	asCheck(e.RegisterGlobalFunction("void pause()", asFUNCTION(pauseConsole), asCALL_CDECL));

	e.SetDefaultNamespace("");
}

} // namespace sn

