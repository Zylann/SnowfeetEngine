#ifndef __HEADER_SN_AS_VECTOR__
#define __HEADER_SN_AS_VECTOR__

#include "../../angelscript/addons/aatc/aatc_common.hpp"

#include "as_math.hpp"
#include "../Vector2.hpp"
#include "../../app/ScriptEngine.hpp"


//------------------------------------------------------------------------------
// Specialization for Vector2
// TODO Have hash functions for any vector, and not just in the binding
template <> class aatc_functor_hash<sn::Vector<sn::s32, 2> >
{
public:
    std::size_t operator()(const sn::Vector<sn::s32,2>& v) const
    {
        return sn::getHash(v);
    }
};

// TODO AATC functor for < operator

namespace sn
{

//------------------------------------------------------------------------------
template <typename T, unsigned int N>
void Vector_defaultConstructor(Vector<T,N> *self)
{
    new(self)Vector<T,N>();
}

//------------------------------------------------------------------------------
template <typename T, unsigned int N>
void Vector_copyConstructor(const Vector<T,N> &other, Vector<T,N> *self)
{
    new(self)Vector<T,N>(other);
}

//------------------------------------------------------------------------------
template <typename T, unsigned int N>
void Vector_listConstructor(T *list, Vector<T,N> *self)
{
    new(self)Vector<T,N>(list);
}

//------------------------------------------------------------------------------
template <typename T>
void Vector2_initConstructor(T x, T y, Vector<T, 2> *self)
{
    new(self)Vector<T, 2>(x, y);
}

//------------------------------------------------------------------------------
template <typename T>
void Vector3_initConstructor(T x, T y, T z, Vector<T, 3> *self)
{
    new(self)Vector<T, 3>(x, y, z);
}

//------------------------------------------------------------------------------
template <typename T>
void Vector4_initConstructor(T x, T y, T z, T w, Vector<T, 4> *self)
{
    new(self)Vector<T, 4>(x, y, z, w);
}

//------------------------------------------------------------------------------
template<typename T>
void register_Vector2Specific(asIScriptEngine & e, std::string c, std::string t)
{
    asCheck(e.RegisterObjectBehaviour(
        c.c_str(), asBEHAVE_CONSTRUCT,
        ("void f(" + t + "," + t + ")").c_str(),
        asFUNCTION(Vector2_initConstructor<T>), asCALL_CDECL_OBJLAST
    ));

    //aatc_register_container_tempspec_vector<Vector<T,2>, 1, 0, 1>(&e, c.c_str());
    //aatc_register_container_tempspec_list<Vector<T, 2>, 1, 0, 1>(engine, c);
    //aatc_register_container_tempspec_set<Vector<T, 2>, 1, 0, 1>(engine, c);
    //aatc_register_container_tempspec_unordered_set<Vector<T, 2>, 1, 0, 1>(&e, c.c_str());
}

//------------------------------------------------------------------------------
inline void register_Vector2iSpecific(asIScriptEngine & e, std::string c)
{
    aatc_Register_aatc_func_hash_value<Vector<s32, 2>>(&e, c.c_str());
}

//------------------------------------------------------------------------------
template<typename T>
void register_Vector3Specific(asIScriptEngine & e, std::string c, std::string t)
{
    asCheck(e.RegisterObjectBehaviour(
        c.c_str(), asBEHAVE_CONSTRUCT,
        ("void f(" + t + "," + t + "," + t + ")").c_str(),
        asFUNCTION(Vector3_initConstructor<T>), asCALL_CDECL_OBJLAST
    ));
}

//------------------------------------------------------------------------------
template<typename T>
void register_Vector4Specific(asIScriptEngine & e, std::string c, std::string t)
{
    asCheck(e.RegisterObjectBehaviour(
        c.c_str(), asBEHAVE_CONSTRUCT,
        ("void f(" + t + "," + t + "," + t + "," + t + ")").c_str(),
        asFUNCTION(Vector4_initConstructor<T>), asCALL_CDECL_OBJLAST
    ));
}

//------------------------------------------------------------------------------
template <typename T, unsigned int N>
void register_Vector(asIScriptEngine & e, std::string c, std::string t, std::vector<std::string> fields)
{
    // Required to get AngelScript macros working, otherwise they take the ',' as an argument separator
    typedef Vector<T, N> V;

    // Register type
    asCheck(e.RegisterObjectType(c.c_str(), sizeof(V), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK));

    // Fields
    for (size_t i = 0; i < fields.size(); ++i)
    {
        asCheck(e.RegisterObjectProperty(c.c_str(),
            (t + " " + fields[i]).c_str(), 
            asOFFSET(V, m_v[i])
        ));
    }

    // Constructors
    asCheck(e.RegisterObjectBehaviour(
        c.c_str(), asBEHAVE_CONSTRUCT,
        "void f()", 
        asFUNCTION((Vector_defaultConstructor<T, N>)), asCALL_CDECL_OBJLAST
    ));
    asCheck(e.RegisterObjectBehaviour(
        c.c_str(), asBEHAVE_CONSTRUCT,
        ("void f(const " + c + " &in)").c_str(), 
        asFUNCTION((Vector_copyConstructor<T, N>)), asCALL_CDECL_OBJLAST
    ));
    asCheck(e.RegisterObjectBehaviour(
        c.c_str(), asBEHAVE_LIST_CONSTRUCT, 
        ("void f(const " + t + " &in) {" + t + "," + t + "}").c_str(), 
        asFUNCTION((Vector_listConstructor<T,N>)), asCALL_CDECL_OBJLAST
    ));

    // Operator overloads
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " &opAddAssign(const " + c + " &in)").c_str(), asMETHODPR(V, operator+=, (const V &), V&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " &opSubAssign(const " + c + " &in)").c_str(), asMETHODPR(V, operator-=, (const V &), V&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " &opMulAssign(const " + c + " &in)").c_str(), asMETHODPR(V, operator*=, (const V &), V&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " &opDivAssign(const " + c + " &in)").c_str(), asMETHODPR(V, operator/=, (const V &), V&), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), ("bool opEquals(const " + c + " &in) const").c_str(), asMETHODPR(V, operator==, (const V &) const, bool), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " opAdd(const " + c + " &in) const").c_str(), asMETHODPR(V, operator+, (const V &) const, V), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " opSub(const " + c + " &in) const").c_str(), asMETHODPR(V, operator-, (const V &) const, V), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " opMul(const " + c + " &in) const").c_str(), asMETHODPR(V, operator*, (const V &) const, V), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (c + " opDiv(const " + c + " &in) const").c_str(), asMETHODPR(V, operator/, (const V &) const, V), asCALL_THISCALL));

    // Methods
    //asCheck(e.RegisterObjectMethod(c, ("float distanceTo(const " + c + " &in) const").c_str(), asMETHOD(Vector<T,N>, getDistanceFrom), asCALL_THISCALL));

    // Properties
    asCheck(e.RegisterObjectMethod(c.c_str(), "float get_length() const", asMETHOD(V, getLength), asCALL_THISCALL));
    //asCheck(e.RegisterObjectMethod(t, "float set_length(float l) const", asMETHOD(Vector2i, setLength), asCALL_THISCALL));
    asCheck(e.RegisterObjectMethod(c.c_str(), (t + " get_lengthSQ() const").c_str(), asMETHOD(V, getLengthSQ), asCALL_THISCALL));

    switch (N)
    {
    case 2: register_Vector2Specific<T>(e, c, t); break;
    case 3: register_Vector3Specific<T>(e, c, t); break;
    case 4: register_Vector4Specific<T>(e, c, t); break;
    default: break;
    }
}

} // namespace sn

#endif // __HEADER_SN_AS_VECTOR__


