/*
Variant.cpp
Copyright (C) 2015-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Variant.hpp"

namespace sn
{

Variant::Variant(const VariantTable & o):
    m_type(SN_VT_VARIANT_TABLE)
{
    m_data.pTable = new VariantTable(o);
}

Variant::Variant(const Variant & other):
    m_type(SN_VT_NIL)
{
    *this = other;
}

Variant::~Variant()
{
    reset();
}

void Variant::reset()
{
    switch (m_type)
    {
    case SN_VT_STRING:              delete m_data.pString; break;
    case SN_VT_VARIANT_ARRAY:       delete m_data.pArray; break;
    case SN_VT_VARIANT_DICTIONARY:  delete m_data.pDictionary; break;
    case SN_VT_VARIANT_TABLE:       delete m_data.pTable; break;
    default: break;
    }
    m_type = SN_VT_NIL;
#ifdef SN_BUILD_DEBUG
    memset(&m_data, 0, sizeof(VariantData));
#endif
}

void Variant::reset(VariantType t)
{
    reset();
    m_type = t;
    switch (m_type)
    {
    case SN_VT_BOOL:
    case SN_VT_INT:
    case SN_VT_FLOAT:
        memset(&m_data, 0, sizeof(VariantData));
    case SN_VT_STRING:              m_data.pString = new String(); break;
    case SN_VT_VARIANT_ARRAY:       m_data.pArray = new VariantArray(); break;
    case SN_VT_VARIANT_DICTIONARY:  m_data.pDictionary = new VariantDictionary(); break;
    case SN_VT_VARIANT_TABLE:       m_data.pTable = new VariantTable(); break;
    default: break;
    }
}

Variant & Variant::operator=(bool b)
{
    reset();
    m_type = SN_VT_BOOL;
    m_data.vBool = b;
    return *this;
}

Variant & Variant::operator=(s32 n)
{
    reset();
    m_type = SN_VT_INT;
    m_data.vInt = n;
    return *this;
}

Variant & Variant::operator=(f32 n)
{
    reset();
    m_type = SN_VT_FLOAT;
    m_data.vFloat = n;
    return *this;
}

Variant & Variant::operator=(const String & str)
{
    reset();
    m_type = SN_VT_STRING;
    m_data.pString = new String(str);
    return *this;
}

Variant & Variant::operator=(const VariantArray & va)
{
    reset();
    m_type = SN_VT_VARIANT_ARRAY;
    m_data.pArray = new VariantArray(va);
    return *this;
}

Variant & Variant::operator=(const VariantDictionary & vd)
{
    reset();
    m_type = SN_VT_VARIANT_DICTIONARY;
    m_data.pDictionary = new VariantDictionary(vd);
    return *this;
}

Variant & Variant::operator=(const VariantTable & vt)
{
    reset();
    m_type = SN_VT_VARIANT_TABLE;
    m_data.pTable = new VariantTable(vt);
    return *this;
}

Variant & Variant::operator=(const Variant & other)
{
    reset();
    switch (other.m_type)
    {
    case SN_VT_BOOL:                m_data.vBool =        other.m_data.vBool; break;
    case SN_VT_INT:                 m_data.vInt =         other.m_data.vInt; break;
    case SN_VT_FLOAT:               m_data.vFloat =       other.m_data.vFloat; break;
    case SN_VT_STRING:              m_data.pString =      new String(*other.m_data.pString); break;
    case SN_VT_VARIANT_ARRAY:       m_data.pArray =       new VariantArray(*other.m_data.pArray); break;
    case SN_VT_VARIANT_DICTIONARY:  m_data.pDictionary =  new VariantDictionary(*other.m_data.pDictionary); break;
    case SN_VT_VARIANT_TABLE:       m_data.pTable =       new VariantTable(*other.m_data.pTable); break;
    }
    return *this;
}

bool Variant::operator==(const Variant & other) const
{
    if (m_type != other.m_type)
        return false;
    if (m_type == SN_VT_NIL)
        return true;

    switch (m_type)
    {
    case SN_VT_BOOL:                return m_data.vBool == other.m_data.vBool; break;
    case SN_VT_INT:                 return m_data.vInt == other.m_data.vInt; break;
    case SN_VT_FLOAT:               return m_data.vFloat == other.m_data.vFloat; break;
    case SN_VT_STRING:              return *m_data.pString == *other.m_data.pString; break;
    case SN_VT_VARIANT_ARRAY:       return *m_data.pArray == *other.m_data.pArray; break;
    case SN_VT_VARIANT_DICTIONARY:  return *m_data.pDictionary == *other.m_data.pDictionary; break;
    case SN_VT_VARIANT_TABLE:       return *m_data.pTable == *other.m_data.pTable; break;
    default: return false;
    }
}

//Variant & Variant::operator[](size_t index)
//{
//    switch (m_type)
//    {    
//    case SN_VT_VARIANT_DICTIONARY:
//        // The key must be a string and the type can't be converted
//        SN_ASSERT(false, "Cannot index or convert VariantDictionary when indexing with an integer");
//        break;
//
//    case SN_VT_VARIANT_TABLE:
//        return (*m_data.pTable)[Variant((s32)index)];
//
//    default:
//        if (m_type != SN_VT_VARIANT_ARRAY)
//        {
//            // Convert to VariantArray, which is the closest match
//            reset();
//            m_data.pArray = new VariantArray();
//        }
//        if (m_data.pArray->size() < index)
//            m_data.pArray->resize(index + 1);
//        return (*m_data.pArray)[index];
//    }
//}
//
//Variant & Variant::operator[](const String & fieldName)
//{
//    // TODO
//}
//
//Variant & Variant::operator[](const Variant & key)
//{
//    // TODO
//}

union MarshallFloat
{
    float f;
    u32 i;
};

size_t Variant::getHash() const
{
    switch (m_type)
    {
    case SN_VT_NIL:
        return 0;

    case SN_VT_BOOL:
        return m_data.vBool ? 1 : 0;

    case SN_VT_INT:
        return m_data.vInt;

    case SN_VT_FLOAT:
    {
        MarshallFloat mf;
        mf.f = m_data.vFloat;
        return mf.i;
    }

    case SN_VT_STRING:
        return std::hash<String>()(*m_data.pString);

    // TODO Have hash functions for the following containers.
    // At the moment, addresses are used.

    case SN_VT_VARIANT_ARRAY:
        return (size_t)m_data.pArray;
        //return std::hash<VariantArray>()(*m_data.pArray);

    case SN_VT_VARIANT_DICTIONARY:
        return (size_t)m_data.pDictionary;
        //return std::hash<VariantDictionary>()(*m_data.pDictionary);

    case SN_VT_VARIANT_TABLE:
        return (size_t)m_data.pTable;
        //return std::hash<VariantTable>()(*m_data.pTable);

    default: return 0;
    }
}

std::string toString(VariantType vt)
{
    switch (vt)
    {
    case SN_VT_NIL:                 return "nil"; break;
    case SN_VT_BOOL:                return "bool"; break;
    case SN_VT_INT:                 return "int"; break;
    case SN_VT_FLOAT:               return "float"; break;
    case SN_VT_STRING:              return "string"; break;
    case SN_VT_VARIANT_ARRAY:       return "array"; break;
    case SN_VT_VARIANT_DICTIONARY:  return "dictionary"; break;
    case SN_VT_VARIANT_TABLE:       return "table"; break;
    default:                        return "undefined"; break;
    }
}

std::string toString(const Variant & v)
{
    if (v.isNil())
        return "Variant(nil)";
    std::stringstream ss;
    ss << toString(v.getType()) << "(";
    switch (v.getType())
    {
    case SN_VT_BOOL:                ss << v.getBool(); break;
    case SN_VT_INT:                 ss << v.getInt(); break;
    case SN_VT_FLOAT:               ss << v.getFloat(); break;
    case SN_VT_STRING:              ss << v.getString(); break;
    case SN_VT_VARIANT_ARRAY:       ss << v.getArray().size() << " elements"; break;
    case SN_VT_VARIANT_DICTIONARY:  ss << v.getDictionary().size() << " elements"; break;
    case SN_VT_VARIANT_TABLE:       ss << v.getTable().size() << " elements"; break;
    }
    ss << ")";
    return ss.str();
}

} // namespace sn

