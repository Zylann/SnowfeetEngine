#include <iostream>
#include <fstream>
#include <vector>

class Processor
{
public:
	Processor():
		m_classMark(false)
	{
		defineToken("//");
		defineToken("/*");
		defineToken("*/");
		defineToken("{");
		defineToken("}");
		defineToken("class");
		defineToken("SN_CLASS");
	}
	
	void process(const std::string & str, size_t pos = 0)
	{
		while(pos < str.size())
		{
			/*if (isStr(str, pos, "namespace"))
			{
				parseNamespace(str, pos)
			}
			else */if(isStr(str, pos, "//"))
			{
				skipLine(str, pos);
			}
			else if(isStr(str, pos, "/*"))
			{
				skipBlockComment(str, pos);
			}
			else if (str[pos] == '"')
			{
				skipQuotedString(str, pos);
			}
			else if(isStr(str, pos, "SN_CLASS"))
			{
				m_classMark = true;
			}
			else if (m_classMark && isStr(str, pos, "class"))
			{
				parseClass(str, pos);
				m_classMark = false;
			}
			
			++pos;
		}
	}

private:
	static bool isStr(const std::string & a, size_t pos, const std::string & b)
	{
		const char * pa = &a[pos];
		const char * pb = &b[0];
		for(size_t i = 0; i < b.size(); ++i)
		{
			if(pa[i] != pb[i])
			{
				return false;
			}
		}
		return true;
	}

	static bool isStrAdv(const std::string & a, size_t & inout_pos, const std::string & b)
	{
		if (isStr(a, inout_pos, b))
		{
			inout_pos += b.size();
			return true;
		}
		return false;
	}

	static bool isWhitespace(char c)
	{
		return c == ' ' || c == '\n' || c == '\r' || c == '\t';
	}

	void defineToken(const std::string & str)
	{
		m_tokens.push_back(str);
	}

	void skipLine(const std::string & str, size_t pos)
	{
		while (pos < str.size() && str[pos] != '\n' && str[pos] != '\r')
		{
			++pos;
		}
	}

	void skipBlockComment(const std::string & str, size_t pos)
	{
		while (pos < str.size())
		{
			if (isStr(str, pos, "*/"))
			{
				pos += 2;
				break;
			}
		}
	}

	void skipQuotedString(const std::string & str, size_t & inout_pos)
	{
		size_t pos = inout_pos;
		while (pos < str.size())
		{
			++pos;
			char c = str[pos];
			if (c == '"')
			{
				if (str[pos - 1] != '\\')
					break;
			}
		}
		inout_pos = pos;
	}

	void parseClass(const std::string & str, size_t pos)
	{
		std::string name;
		std::vector<std::string> inheritedClasses;
		parseNextName(str, pos, name);

		while (pos < str.size())
		{
			char c = str[pos];
			++pos;

			if (c == ':')
			{
				parseInheritance(str, pos, inheritedClasses);
			}
			if (c == '{')
			{
				parseClassBody(str, pos);
				break;
			}
			else if (c == ';')
			{
				break;
			}
		}
	}

	void parseInheritance(const std::string & str, size_t pos, std::vector<std::string> & out_classes)
	{
		while (pos < str.size())
		{
			skipWhitespace(str, pos);
			char c = str[pos];
			if (isStrAdv(str, pos, "public"))
			{
				continue;
			}
			else if (isStrAdv(str, pos, "protected"))
			{
				continue;
			}
			else if (isStrAdv(str, pos, "private"))
			{
				continue;
			}
			else if (isalpha(str[pos]))
			{
				std::string name;
				// TODO Handle template names
				parseNextName(str, pos, name);
				out_classes.push_back(name);
			}
			else if (c == '{' || c == ';')
			{
				break;
			}
		}
	}

	void parseClassBody(const std::string & str, size_t pos)
	{
		while (pos < str.size())
		{
			// TODO
		}
	}

	void parseNextName(const std::string & str, size_t & inout_pos, std::string & out_name)
	{
		size_t begin = inout_pos;
		while (begin < str.size() && !isalpha(str[begin]))
			++begin;

		size_t end = begin;
		while (end < str.size() && isalnum(str[end]))
			++end;

		out_name = str.substr(begin, end);

		inout_pos = end;
	}

	void skipWhitespace(const std::string & str, size_t & inout_pos)
	{
		size_t i = inout_pos;
		while (i < str.size())
		{
			if (isWhitespace(str[i]))
				++i;
		}
		inout_pos = i;
	}
	
private:
	std::vector<std::string> m_tokens;
	bool m_classMark;
};

bool readAllFile(const std::string & filePath, std::string & out)
{
	std::ifstream ifs(filePath, std::ios::in | std::ios::binary);
	if (!ifs)
		return false;

	ifs.seekg(0, std::ios::end);
	std::streamoff len = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	out.resize(static_cast<size_t>(len));
	ifs.read(&out[0], len);
	ifs.close();

	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		return -1;
	std::string fileName = argv[2];

	std::string content;
	if (!readAllFile(fileName, content))
		return -1;

	Processor processor;
	processor.process(content);

	return 0;
}
