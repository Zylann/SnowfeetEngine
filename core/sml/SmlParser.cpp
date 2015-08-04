#include <sstream>
#include "SmlParser.h"

namespace sn
{

//------------------------------------------------------------------------------
SmlParser::SmlParser() :
	m_lastCharacter('%')
{
}

//------------------------------------------------------------------------------
bool SmlParser::parseValue(std::istream & input, Variant & out_value)
{
	bool found = false;

	while (!found && !input.eof())
	{
		const char c = input.peek();

		switch (c)
		{
		case '/':
		case '#':
			parseComment(input);
			found = true;
			break;

		case '{':
			m_lastCharacter = input.get();
			out_value.setDictionary();
			parseObject(input, out_value.getDictionary());
			found = true;
			break;

		case '[':
		case '(':
			m_lastCharacter = input.get();
			out_value.setArray();
			parseArray(input, out_value.getArray());
			found = true;
			break;

		case '"':
			m_lastCharacter = input.get();
			out_value.setString("");
			parseString(input, out_value.getString());
			found = true;
			break;

		case ' ':
		case '\t':
			m_lastCharacter = input.get();
			break;

		case '\n':
		case '\r':
			m_lastCharacter = input.get();
			break;

		default:
			if (isalpha(c))
			{
				parseTypedObject(input, out_value);
				found = true;
			}
			else if (isdigit(c) || c == '-')
			{
				parseNumber(input, out_value);
				found = true;
			}
			else
			{
				// Value not found
				m_lastCharacter = input.get();
			}
			break;
		}
	}

	return found;
}

//------------------------------------------------------------------------------
void SmlParser::parseNumber(std::istream & input, Variant & out_value)
{
	std::stringstream ss;
	//const char lastChar = input.peek();
	//if (isdigit(lastChar) || lastChar == '-')
	//{
	//	ss << lastChar;
	//}

	VariantType numberType = SN_VT_INT;

	while (!input.eof())
	{
		const char c = input.peek();
		//m_lastCharacter = input.get();
		//const char c = m_lastCharacter;

		if (!isNumberChar(c))
		{
			break;
		}

		if (c == '.')
		{
			numberType = SN_VT_FLOAT;
		}

		ss << c;
		m_lastCharacter = input.get();
	}

	if (numberType == SN_VT_FLOAT)
	{
		f32 v = 0;
		ss >> v;
		out_value.setFloat(v);
	}
	else
	{
		s32 v = 0;
		ss >> v;
		out_value.setInt(v);
	}
}

//------------------------------------------------------------------------------
void SmlParser::parseComment(std::istream & input)
{
	while (!input.eof())
	{
		char c = input.get();
		if (c == '\n' || c == '\r')
			break;
	}
	while (!input.eof())
	{
		char c = input.get();
		if (c != '\n' && c != '\r')
			break;
	}
}

//------------------------------------------------------------------------------
void SmlParser::parseObject(std::istream & input, Variant::Dictionary & out_value)
{
	while (!input.eof())
	{
		const char c = input.peek();

		if (isalpha(c) || c == '"')
		{
			// Parse key
			std::string key;
			bool allowSpaces = (c == '"');
			parseKey(input, key, allowSpaces);
			Variant & value = out_value[key]; // Access creates the pair

			// Make sure to read after a ':'
			if (m_lastCharacter != ':')
			{
				skipUntil(input, ':');
			}

			// Parse value
			parseValue(input, value);

			// If the value parsing encountered the end of the object
			//if (m_lastCharacter == '}')
			//{
			//	m_lastCharacter = input.get();
			//	break;
			//}
		}
		else if (c == '}')
		{
			// End of object
			m_lastCharacter = input.get();
			break;
		}
		else
		{
			m_lastCharacter = input.get();
		}
	}
}

//------------------------------------------------------------------------------
/// \brief Starts at a begin quote or an alphanumeric character, ends at a closing quote or non-alphanumeric character
void SmlParser::parseKey(std::istream & input, std::string & out_value, bool allowSpaces)
{
	//out_value = m_lastCharacter;
	bool next = true;
	while (next && !input.eof())
	{
		const char c = input.peek();

		switch (c)
		{
		case '"':
			if (m_lastCharacter == '"')
				next = false;
			else
				m_lastCharacter = input.get();
			break;

		case ' ':
			if (allowSpaces)
			{
				out_value += c;
				m_lastCharacter = input.get();
			}
			else
				next = false;
			break;

		default:
			if (isKeyChar(c))
			{
				out_value += c;
				m_lastCharacter = input.get();
			}
			else
				next = false;
			break;
		}
	}
}

//------------------------------------------------------------------------------
void SmlParser::parseTypedObject(std::istream & input, Variant & out_value)
{
	// Parse type
	std::string typeKey;
	parseKey(input, typeKey, false);

	if (typeKey == "true")
	{
		out_value.setBool(true);
	}
	else if (typeKey == "false")
	{
		out_value.setBool(false);
	}
	else if (typeKey == "null")
	{
		out_value.reset();
	}
	//else
	//{
	//	// TODO FIXME There is a leading space in type names!
	//	// This is a custom typed object
	//	out_value.resetTypedObject();
	//	TypedObject & o = *out_value.m_data.pCustom;
	//	o.typeName = typeKey;
	//	// Parse value
	//	// TODO Can the value be a typed object itself?
	//	parseValue(input, o.value);
	//}
}

//------------------------------------------------------------------------------
void SmlParser::parseArray(std::istream & input, Variant::Array & out_value)
{
	bool next = true;
	while (next && !input.eof())
	{
		const char c = input.peek();

		switch (c)
		{
		case ']':
		case ')':
			next = false;
			m_lastCharacter = input.get();
			break;

		case '[':
		case '(':
			m_lastCharacter = input.get();
			break;

		case ',':
			m_lastCharacter = input.get();
			break;

		default:
			if (!isWhiteSpace(c))
			{
				out_value.push_back(Variant());
				if (!parseValue(input, out_value.back()))
				{
					out_value.pop_back();
				}
			}
			else
			{
				m_lastCharacter = input.get();
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
void SmlParser::parseString(std::istream & input, Variant::String & out_value)
{
	bool escaping = false;
	bool inString = true;
	while (inString && !input.eof())
	{
		char c = input.get();
		switch (c)
		{
		case '"':
			if (escaping)
			{
				out_value += c;
				escaping = false;
			}
			else
				inString = false;
			break;

		case '\\':
			if (escaping)
			{
				out_value += c;
				escaping = false;
			}
			else
				escaping = true;
			break;

		default:
			if (escaping)
			{
				switch (c)
				{
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				default: break;
				}
			}
			out_value += c;
			break;
		}
	}
}

//------------------------------------------------------------------------------
void SmlParser::skipUntil(std::istream & input, char c)
{
	while (!input.eof())
	{
		if (input.get() == c)
			break;
	}
}

//------------------------------------------------------------------------------
bool SmlParser::isWhiteSpace(char c)
{
	return !!isspace(c);
}

//------------------------------------------------------------------------------
bool SmlParser::isKeyChar(char c)
{
	return isalpha(c) || isdigit(c) || c == '_' || c == '.';
}

//------------------------------------------------------------------------------
bool SmlParser::isNumberChar(char c)
{
	return isdigit(c) || c == '-' || c == '.';
}

} // namespace sn

