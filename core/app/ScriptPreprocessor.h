#ifndef __HEADER_SN_SCRIPTPREPROCESSOR__
#define __HEADER_SN_SCRIPTPREPROCESSOR__

#include <string>
#include <vector>
#include <core/types.h>
#include <core/util/Parser.h>

namespace sn
{

class ScriptPreprocessor : public Parser
{
public:
	struct RequireItem
	{
		std::string fileName;
		u32 line;

		RequireItem(): line(0) {}
		RequireItem(const std::string a_fileName, u32 a_line) : fileName(a_fileName), line(a_line) {}
	};

	ScriptPreprocessor();

	void run(std::string & out_input);

	const std::vector<RequireItem> & getRequireItems() const { return m_requireItems; }

private:
	void processDirective(std::string & out_input, size_t & out_pos);
	//void processIfdefBlock(std::string & out_input, size_t pos);

private:
	std::vector<RequireItem> m_requireItems;
	u32 m_lineNumber;

};

} // namespace sn

#endif // __HEADER_SN_SCRIPTPREPROCESSOR__