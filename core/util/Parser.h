#ifndef __HEADER_SN_PARSER__
#define __HEADER_SN_PARSER__

#include <string>
#include <cassert>

namespace sn
{
	
/// \brief Base class for building simple parsers
class Parser
{
public:
	
protected:
	typedef bool(*CharTestFunction)(char);

	static bool parseQuotedString(const std::string & input, size_t & out_pos, std::string & out_str);
	static bool strEqualsAdv(const std::string & input, size_t & out_pos, const std::string & str);
	static size_t skipWhitespace(const std::string & input, size_t pos);
	static size_t skipToNextLine(const std::string & input, size_t pos);
	static bool parseName(const std::string & input, size_t & out_pos, std::string & out_str);
	
	static void retrieveLocation(const std::string & input, size_t pos, size_t & out_line, size_t & out_column);

	static inline bool skipUntil(const std::string & input, size_t & out_pos, char c)
	{
		while (out_pos < input.size())
		{
			if (input[out_pos++] == c)
				return true;
		}
		return false;
	}

	static inline bool skipUntil(const std::string & input, size_t & out_pos, CharTestFunction f)
	{
		assert(f != nullptr, "Function pointer cannot be null");
		while (out_pos < input.size())
		{
			if (f(input[out_pos++]))
				return true;
		}
		return false;
	}

	static inline bool skipUntilNot(const std::string & input, size_t & out_pos, CharTestFunction f)
	{
		assert(f != nullptr, "Function pointer cannot be null");
		while (out_pos < input.size())
		{
			if (!f(input[out_pos++]))
				return true;
		}
		return false;
	}

	static inline bool isEOL(char c)            { return c == '\n' || c == '\r'; }
	static inline bool isWhiteSpace(char c)     { return isspace(c); }
	static inline bool isAlpha(char c)          { return isalpha(c); }
	static inline bool isAlphanumeric(char c)   { return isalnum(c); }

};
	
} // namespace sn

#endif // __HEADER_SN_PARSER__

