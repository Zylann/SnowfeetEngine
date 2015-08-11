/*
Variant.h
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_VARIANT__
#define __HEADER_SN_VARIANT__

#include <core/types.h>
#include <core/math/Vector2.h>
#include <core/math/Vector3.h>
#include <core/math/Quaternion.h>
#include <core/math/Matrix4.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace sn
{

enum VariantTypeID
{
    SN_VT_NIL = 0, // TODO Change to NULL for naming coherence

    SN_VT_BOOL,
    SN_VT_INT,
    SN_VT_FLOAT,

    SN_VT_STRING,

    SN_VT_ARRAY, // unsigned int => Variant
    SN_VT_DICTIONARY, // string => Variant

    SN_VT_COUNT // Keep last
};

struct VariantType
{
	VariantType(VariantTypeID a_id): id(a_id), extra(0) {}

	bool operator==(u32 a_id) const { return id == a_id; }
	bool operator!=(u32 a_id) const { return id != a_id; }

	/// \brief First-order type ID
	u16 id;
	/// \brief Additional type information, mostly usage-dependant
	u16 extra;
};

class Variant;

std::string SN_API toString(VariantType vt);
std::string SN_API toString(const Variant & v);

/// \brief Template-free implementation of a variant type
class SN_API Variant
{
public:

    typedef std::string String;
    typedef std::vector<Variant> Array;
    typedef std::unordered_map<std::string, Variant> Dictionary;

    union VariantData
    {
        bool vBool;
        s32 vInt;
        f32 vFloat;
        String * pString;
        Array * pArray;
        Dictionary * pDictionary;
    };

    //--------------------------------------
    // Constructors
    //--------------------------------------

    Variant() :                       m_type(SN_VT_NIL)         {}
    Variant(bool b) :                 m_type(SN_VT_BOOL)        { m_data.vBool = b; }
    Variant(s32 n) :                  m_type(SN_VT_INT)         { m_data.vInt = n; }
    Variant(f32 n) :                  m_type(SN_VT_FLOAT)       { m_data.vFloat = n; }
    Variant(const String & s) :       m_type(SN_VT_STRING)      { m_data.pString = new String(s); }
    Variant(const Array & o) :        m_type(SN_VT_ARRAY)       { m_data.pArray = new Array(o); }
    Variant(const Dictionary & o) :   m_type(SN_VT_DICTIONARY)  { m_data.pDictionary = new Dictionary(o); }
	// TODO C++11 Move constructor

    Variant(const Variant & other);

    ~Variant();

    //--------------------------------------
    // Methods
    //--------------------------------------

    /// \brief Clears contents and sets the type to NIL.
    void reset();

    /// \brief Clears contents and sets the given type.
    void reset(VariantType t);

    inline VariantType getType() const { return m_type; }

	void Variant::assertType(VariantType t) const
	{
		SN_ASSERT(m_type.id == t.id, "Variant " << toString(t) << " expected, got " << toString(*this));
	}

    bool getBool() const;
    s32 getInt() const;
    f32 getFloat() const;

    const String & getString() const;
    const Array & getArray() const;
    const Dictionary & getDictionary() const;

	Variant::String & Variant::getString()
	{
		assertType(SN_VT_STRING);
		return *m_data.pString;
	}

	Variant::Array & Variant::getArray()
	{
		assertType(SN_VT_ARRAY);
		return *m_data.pArray;
	}

	Variant::Dictionary & Variant::getDictionary()
	{
		assertType(SN_VT_DICTIONARY);
		return *m_data.pDictionary;
	}

    void setBool(bool b);
    void setInt(s32 n);
    void setFloat(f32 f);
    void setString(const String & str);
    void setArray(const Array & va);
    void setDictionary(const Dictionary & vd);

    /// \brief Moves the contents of another variant to this one.
    /// This can be faster than copy the values.
    /// Previous data is erased.
    /// \param other: variant to grab. Will be left null.
    void grab(Variant & other);

    size_t getHash() const;

    //--------------------------------------
    // Operators
    //--------------------------------------

    Variant & operator=(bool b)                  { setBool(b); return *this; }
    Variant & operator=(s32 n)                   { setInt(n); return *this; }
    Variant & operator=(f32 n)                   { setFloat(n); return *this; }
    Variant & operator=(const String & str)      { setString(str); return *this; }
    Variant & operator=(const Array & va)        { setArray(va); return *this; }
    Variant & operator=(const Dictionary & vd)   { setDictionary(vd); return *this; }

    Variant & operator=(const Variant & other);

    bool operator==(const Variant & other) const;

    Variant & operator[](size_t index);
    Variant & operator[](const String & fieldName);

    const Variant & operator[](size_t index) const;
    const Variant & operator[](const String & fieldName) const;

    //--------------------------------------
    // Helpers
    //--------------------------------------

    /// \brief If the variant is not an array, it is changed to an empty array.
    void setArray();
    /// \brief If the variant is not a dictionary, it is changed to an empty one.
    void setDictionary();

    inline bool isNil() const         { return m_type.id == SN_VT_NIL; }
    inline bool isBool() const        { return m_type.id == SN_VT_BOOL; }
    inline bool isInt() const         { return m_type.id == SN_VT_INT; }
    inline bool isFloat() const       { return m_type.id == SN_VT_FLOAT; }
    inline bool isString() const      { return m_type.id == SN_VT_STRING; }
    inline bool isArray() const       { return m_type.id == SN_VT_ARRAY; }
    inline bool isDictionary() const  { return m_type.id == SN_VT_DICTIONARY; }

private:

    VariantType m_type;
    VariantData m_data;
};

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

