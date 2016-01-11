#ifndef __HEADER_SN_SCRIPTPREPROCESSOR__
#define __HEADER_SN_SCRIPTPREPROCESSOR__

#include <string>
#include <unordered_map>
#include <stack>

namespace sn
{
#if 0
class ScriptPreprocessor
{
public:
	ScriptPreprocessor();

	void process(std::string & str, const std::string & fileName);
	void setDefine(const std::string & name, const std::string & value);
	bool isDefined(const std::string & macroName);

private:
	void processLine(std::string & str, size_t & i);
	void parseDirective(std::string & str, size_t & i);
	void parseWord(const std::string & str, size_t & i, std::string & out_word);
	void parseExpression(const std::string & str, size_t & i, std::string & out_expr);
	void disableBlock(std::string & str, size_t & i);

	u32 retrieveLineNumber(const std::string & str, size_t pos);

private:
	std::unordered_map<std::string, std::string> m_macros;
	char m_beginChar;
	std::string m_fileName;

};
#endif
} // namespace sn

#endif // __HEADER_SN_SCRIPTPREPROCESSOR__

