#ifndef __HEADER_SML_PARSER__
#define __HEADER_SML_PARSER__

#include <istream>
#include <core/util/Variant.h>

namespace sn
{

/// \brief Parses SML byte stream into Variants.
/// The input can be either JSON or SML.
class SN_API SmlParser
{
public:
	SmlParser();

	bool parseValue(std::istream & input, Variant & out_value);

private:
	void parseComment(std::istream & input);
	void parseObject(std::istream & input, Variant::Dictionary & out_value);
	void parseKey(std::istream & input, std::string & out_value, bool allowSpaces);
	void parseTypedObject(std::istream & input, Variant & out_value);
	void parseArray(std::istream & input, Variant::Array & out_value);
	void parseNumber(std::istream & input, Variant & out_value);
	void parseString(std::istream & input, Variant::String & out_value);

	void skipUntil(std::istream & input, char c);

	bool isWhiteSpace(char c);
	bool isKeyChar(char c);
	bool isNumberChar(char c);

private:
	char m_lastCharacter;

};

} // namespace sml

#endif // __HEADER_SML_PARSER__

