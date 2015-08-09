#include "SmlWriter.h"

namespace sn
{

//------------------------------------------------------------------------------
void SmlWriter::writeValue(std::ostream & os, const Variant & value)
{
	switch (value.getType().id)
	{
	case SN_VT_BOOL:
		writeBool(os, value.getBool());
		break;

	case SN_VT_INT: os << value.getInt(); break;
	case SN_VT_FLOAT: os << value.getFloat(); break;

	case SN_VT_STRING:
		writeString(os, value.getString());
		break;

	case SN_VT_ARRAY:
		writeArray(os, value.getArray());
		break;

	case SN_VT_DICTIONARY:
		writeObject(os, value.getDictionary());
		break;

	//case SN_VT_TYPEDOBJECT:
	//	writeTypedObject(os, value.getTypedObject());
	//	break;

	default:
		os << "null";
		break;
	}
}

//------------------------------------------------------------------------------
void SmlWriter::writeBool(std::ostream & os, bool v)
{
	if (v)
		os << "true";
	else
		os << "false";
}

//------------------------------------------------------------------------------
void SmlWriter::writeString(std::ostream & os, const Variant::String & s)
{
    u32 i_start = 0;
    for (u32 i = 0; i < s.size(); ++i)
    {
        char c = s[i];
        if (isWhitespace(c))
        {
            if (c == '\n')
            {
                os << s.substr(i_start, i - i_start);
                os << "\\n";
                i_start = i + 1;
            }
            else if (c == '\r')
            {
                os << s.substr(i_start, i - i_start);
                os << "\\r";
                i_start = i + 1;
            }
            else if (c == '\t')
            {
                os << s.substr(i_start, i - i_start);
                os << "\\t";
                i_start = i + 1;
            }
        }
    }
    if (i_start != s.size())
        os << s.substr(i_start, s.size() - i_start);
}

//------------------------------------------------------------------------------
void SmlWriter::writeArray(std::ostream & os, const Variant::Array & a)
{
	os << "[";

	for (unsigned int i = 0; i < a.size(); ++i)
	{
		writeValue(os, a[i]);
		if (i != a.size() - 1)
		{
			writeSeparator(os);
		}
	}

	os << "]";
}

//------------------------------------------------------------------------------
void SmlWriter::writeObject(std::ostream & os, const Variant::Dictionary & o)
{
	os << "{";

	bool sep = false;
	for (auto it = o.begin(); it != o.end(); ++it)
	{
		if (sep)
		{
			writeSeparator(os);
		}

		os << it->first << ":";

        const Variant & v = it->second;
        if (v.isString())
        {
            // String values are quoted
            os << '"';
    		writeValue(os, it->second);
            os << '"';
        }
        else
        {
    		writeValue(os, it->second);
        }

		sep = true;
	}

	os << "}";
}

//------------------------------------------------------------------------------
//void Writer::writeTypedObject(std::ostream & os, const TypedObject & to)
//{
//	os << to.typeName;
//	writeValue(os, to.value);
//}

//------------------------------------------------------------------------------
void SmlWriter::writeSeparator(std::ostream & os)
{
	os << ',';
}

} // namespace sn

