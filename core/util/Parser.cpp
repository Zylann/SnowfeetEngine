#include "Parser.h"

namespace sn
{
	
//------------------------------------------------------------------------------
bool Parser::parseQuotedString(const std::string & input, size_t & out_pos, std::string & out_str)
{
	size_t pos = skipWhitespace(input, out_pos);
	while (true)
	{
		if (input[pos++] == '"')
			break;
		if (pos >= input.size())
			return false;
	}

	size_t beginPos = pos;
	std::string str;
	char c;
	while (pos < input.size())
	{
		c = input[pos++];
		if (c == '\\')
		{
			if (pos < input.size())
			{
				c = input[pos++];
				str += c;
			}
		}
		else if (c == '"')
			break;
		else if (isEOL(c))
			return false;
		else
			str += c;
	}

	// The last read character has to be a closing quote
	if (c != '"')
		return false;

	out_str = str;
	out_pos = pos;

	return true;
}

//------------------------------------------------------------------------------
bool Parser::strEqualsAdv(const std::string & input, size_t & out_pos, const std::string & str)
{
	if (out_pos + str.size() > input.size())
		return false;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (input[out_pos + i] != str[i])
			return false;
	}
	out_pos += str.size();
	return true;
}

//------------------------------------------------------------------------------
size_t Parser::skipWhitespace(const std::string & input, size_t pos)
{
	while (pos < input.size())
	{
		if (!isWhiteSpace(input[pos]))
			break;
		++pos;
	}
	return pos;
}

//------------------------------------------------------------------------------
size_t Parser::skipToNextLine(const std::string & input, size_t pos)
{
	while (pos < input.size())
	{
		char c = input[pos];
		if (isEOL(c))
			break;
		++pos;
	}
	while (pos < input.size())
	{
		char c = input[pos];
		if (!isEOL(c))
			break;
		++pos;
	}
	return pos;
}

//------------------------------------------------------------------------------
bool Parser::parseName(const std::string & input, size_t & out_pos, std::string & out_str)
{
	size_t pos = skipWhitespace(input, out_pos);
	if (!isAlpha(input[pos]))
		return false;

	size_t beginPos = pos;

	while (pos < input.size())
	{
		char c = input[pos++];
		if (!isAlphanumeric(c))
			break;
	}

	std::string str = input.substr(beginPos, pos - beginPos + 1);
	out_str = str;
	out_pos = pos;
	return true;
}

//------------------------------------------------------------------------------
void Parser::retrieveLocation(const std::string & input, size_t a_pos, size_t & out_line, size_t & out_column)
{
	if (a_pos >= input.size())
		a_pos = input.size();
	
	if (a_pos <= 2)
	{
		out_line = 0;
		out_column = a_pos;
		return;
	}

	size_t line = 0;
	size_t column = 0;

	size_t pos = a_pos;
	while (pos > 0)
	{
		char c = input[pos--];
		if (c == '\n')
			break;
	}

	column = pos;
	++line;

	while (pos > 0)
	{
		char c = input[pos--];
		if (c == '\n')
			++line;
	}

	out_line = line;
	out_column = column;
}

} // namespace sn

