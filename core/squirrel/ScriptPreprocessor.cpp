#include "ScriptPreprocessor.h"
#include <core/util/stringutils.h>
#include <core/util/Log.h>
#if 0
namespace sn
{

ScriptPreprocessor::ScriptPreprocessor():
	m_beginChar('#')
{
}

bool ScriptPreprocessor::isDefined(const std::string & macroName)
{
	return m_macros.find(macroName) != m_macros.end();
}

void ScriptPreprocessor::process(std::string & str, const std::string & fileName)
{
	m_fileName = fileName;

	if (str.empty())
		return;

	size_t i = 0;

	while (true)
	{
		char c = str[i];
		i = str.find_first_not_of(" \n\r\t", i);
		if (i == std::string::npos)
			break;
		if (str[i] == m_beginChar)
		{
			parseDirective(str, i);
		}
		else
		{
			i = str.find_first_of("\n\r", i);
			if (i == std::string::npos)
				break;
		}
	}
}

void ScriptPreprocessor::processLine(std::string & str, size_t begin, size_t end)
{

}

void ScriptPreprocessor::parseDirective(std::string & str, size_t & i)
{
	size_t begin = i;

	// Skip the beginChar
	if (++i >= str.size())
		return;
	
	std::string directive;
	parseWord(str, i, directive);

	if (directive == "if")
	{
		std::string expression;
		parseExpression(str, i, expression);

		if (!isDefined(expression))
		{
			disableBlock(str, i);
		}
	}
	else
	{
		u32 lineNumber = retrieveLineNumber(str, i);
		SN_ERROR("Unrecognized preprocessor directive '" << directive << "' in file '" << m_fileName << "' near line " << (lineNumber + 1));
		return;
	}
}

void ScriptPreprocessor::disableBlock(std::string & str, size_t & i)
{
	while (i < str.size())
	{
		i = str.find_first_of('\n');
		if (c == m_beginChar)
		{
			std::string directive;
			parseWord(str, i, directive);
			if (directive == "endif")
			{
				break;
			}
		}
		else if (c != '\n')
		{
			str[i] = ' ';
		}
		++i;
	}
}

void ScriptPreprocessor::parseWord(const std::string & str, size_t & i, std::string & out_word)
{
	const char * const spaceChars = " \n\r\t";

	size_t begin = str.find_first_not_of(spaceChars, i);
	if (begin == std::string::npos)
		return;

	size_t end = str.find_first_of(spaceChars, begin);

	out_word = str.substr(begin, end);

	i = end;
}

void ScriptPreprocessor::parseExpression(const std::string & str, size_t & i, std::string & out_expr)
{
	parseWord(str, i, out_expr);
}

u32 ScriptPreprocessor::retrieveLineNumber(const std::string & str, size_t pos)
{
	u32 lineNumber = 0;
	for (u32 i = 0; i <= pos; ++i)
	{
		char c = str[i];
		if (c == '\n')
		{
			++lineNumber;
		}
	}
	return lineNumber;
}

void ScriptPreprocessor::setDefine(const std::string & name, const std::string & value)
{
	m_macros[name] = value;
}

} // namespace sn

#endif