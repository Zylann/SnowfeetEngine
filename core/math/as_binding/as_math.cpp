#include "as_math.hpp"
#include "../../app/ScriptEngine.hpp"
#include "../math.hpp"
#include "../Vector2.hpp"
#include "../Color.hpp"
#include "as_Vector.hpp"
#include "as_Range.hpp"

namespace sn
{

//------------------------------------------------------------------------------
f32 asRand() { return math::randf(); }
f32 asRandMinMax(f32 min, f32 max) { return math::randf(min, max); }
s32 asRandIntMinMax(s32 min, s32 max) { return math::rand(min, max); }

//------------------------------------------------------------------------------
void register_math(asIScriptEngine & e)
{
    // Functions

    asCheck(e.RegisterGlobalFunction("float rand()", asFUNCTIONPR(asRand, (), f32), asCALL_CDECL));
    asCheck(e.RegisterGlobalFunction("float rand(float, float)", asFUNCTIONPR(asRandMinMax, (f32, f32), f32), asCALL_CDECL));
    asCheck(e.RegisterGlobalFunction("int randint(int, int)", asFUNCTIONPR(asRandIntMinMax, (s32, s32), s32), asCALL_CDECL));

    // Objects

    register_Vector<s32, 2>(e, "Vector2i", "int", { "x", "y" });
    register_Vector<f32, 2>(e, "Vector2f", "float", { "x", "y"});

    register_Vector<s32, 3>(e, "Vector3i", "int", { "x", "y", "z" });
    register_Vector<f32, 3>(e, "Vector3f", "float", { "x", "y", "z" });

    register_Vector<s32, 4>(e, "Vector4i", "int", { "x", "y", "z", "w" });
    register_Vector<f32, 4>(e, "Vector4f", "float", { "x", "y", "z", "w" });

    register_Color(e);

    register_Range<s32>(e, "IntRange", "int");
    register_Range<f32>(e, "FloatRange", "float");

    register_IntRect(e);
    register_FloatRect(e);

}

} // namespace sn




