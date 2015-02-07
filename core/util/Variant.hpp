/*
Variant.hpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VARIANT__
#define __HEADER_SN_VARIANT__

#include <core/types.hpp>
#include <core/math/Vector2.hpp>
#include <core/math/Vector3.hpp>
#include <core/math/Quaternion.hpp>
#include <core/math/Matrix4.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace sn
{

enum VariantType
{
    SN_VT_NIL = 0,
    SN_VT_BOOL,
    SN_VT_INT,
    SN_VT_FLOAT,

    SN_VT_STRING,

    SN_VT_ARRAY, // unsigned int => Variant
    SN_VT_DICTIONARY, // string => Variant
    //SN_VT_TABLE, // Variant => Variant

    // TODO Support for VariantTable (Variant => Variant)

    SN_VT_COUNT // Keep last
};

/// \brief Template-free implementation of a variant type
class Variant
{
public:

    typedef std::string String;
    typedef std::vector<Variant> Array;
    typedef std::unordered_map<std::string, Variant> Dictionary;
    //typedef std::unordered_map<Variant, Variant> VariantTable;

    union VariantData
    {
        bool vBool;
        s32 vInt;
        f32 vFloat;
        String * pString;
        Array * pArray;
        Dictionary * pDictionary;
        //VariantTable * pTable;
    };

    Variant() :                       m_type(SN_VT_NIL)         {}
    Variant(bool b) :                 m_type(SN_VT_BOOL)        { m_data.vBool = b; }
    //Variant(u32 n) :                  m_type(SN_VT_INT)         { m_data.vInt = n; }
    Variant(s32 n) :                  m_type(SN_VT_INT)         { m_data.vInt = n; }
    Variant(f32 n) :                  m_type(SN_VT_FLOAT)       { m_data.vFloat = n; }
    Variant(const String & s) :       m_type(SN_VT_STRING)      { m_data.pString = new String(s); }
    Variant(const Array & o) :        m_type(SN_VT_ARRAY)       { m_data.pArray = new Array(o); }
    Variant(const Dictionary & o) :   m_type(SN_VT_DICTIONARY)  { m_data.pDictionary = new Dictionary(o); }
    //Variant(const Table & o);
    Variant(const Variant & other);

    ~Variant();

    /// \brief Clears contents and sets the type to NIL.
    void reset();

    /// \brief Clears contents and sets the given type.
    void reset(VariantType t);

    inline VariantType getType() const { return m_type; }

    inline bool isNil() const         { return m_type == SN_VT_NIL; }
    inline bool isBool() const        { return m_type == SN_VT_BOOL; }
    inline bool isInt() const         { return m_type == SN_VT_INT; }
    inline bool isFloat() const       { return m_type == SN_VT_FLOAT; }
    inline bool isString() const      { return m_type == SN_VT_STRING; }
    inline bool isArray() const       { return m_type == SN_VT_ARRAY; }
    inline bool isDictionary() const  { return m_type == SN_VT_DICTIONARY; }
    //inline bool isTable() const       { return m_type == SN_VT_TABLE; }

    void assertType(VariantType t) const;

    bool getBool() const;
    s32 getInt() const;
    f32 getFloat() const;
    const String & getString() const;
    const Array & getArray() const;
    const Dictionary & getDictionary() const;
    //const VariantTable & getTable() const;

    Variant & operator=(bool b);
    Variant & operator=(s32 n);
    Variant & operator=(f32 n);
    Variant & operator=(const String & str);
    Variant & operator=(const Array & va);
    Variant & operator=(const Dictionary & vd);
    //Variant & operator=(const Table & vt);
    Variant & operator=(const Variant & other);

    bool operator==(const Variant & other) const;

    Variant & operator[](size_t index);
    Variant & operator[](const String & fieldName);
    //Variant & operator[](const Variant & key);

    size_t getHash() const;

private:

    VariantType m_type;
    VariantData m_data;
};

std::string toString(VariantType vt);
std::string toString(const Variant & v);

} // namespace sn

/// Std Hash
namespace std
{
    template <>
    struct hash<sn::Variant>
    {
        // 32-bit integer (x,y) hash
        inline size_t operator()(const sn::Variant & v) const
        {
            return v.getHash();
        }
    };

} // namespace std

#endif // __HEADER_SN_VARIANT__

