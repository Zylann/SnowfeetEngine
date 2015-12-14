#include "ScriptPreprocessor.h"

namespace sn
{

//------------------------------------------------------------------------------
ScriptPreprocessor::ScriptPreprocessor() : m_lineNumber(0)
{}

//------------------------------------------------------------------------------
void ScriptPreprocessor::process(std::string & out_input)
{
	m_lineNumber = 0;
	m_requireItems.clear();

	size_t pos = 0;
	while (pos < out_input.size())
	{
		char c = out_input[pos];
		if (c == '#')
		{
			processDirective(out_input, pos);
		}
		else if (!isWhiteSpace(c))
		{
			pos = skipToNextLine(out_input, pos);
			++m_lineNumber;
		}
		else
		{
			++pos;
		}
	}
}

//------------------------------------------------------------------------------
void ScriptPreprocessor::processDirective(std::string & out_input, size_t pos)
{
	++pos;
	if (pos >= out_input.size())
		return;

	if (strEqualsAdv(out_input, pos, "require"))
	{
		std::string fileName;
		if (parseQuotedString(out_input, pos, fileName))
		{
			m_requireItems.push_back({ fileName, m_lineNumber });
		}
	}
	//else if (strEqualsAdv(out_input, pos, "ifdef"))
	//{
	//	processIfdefBlock(out_input, pos);
	//}
}

//------------------------------------------------------------------------------
//void ScriptPreprocessor::processIfdefBlock(std::string & out_input, size_t pos)
//{
//	// TODO
//}

} // namespace sn

