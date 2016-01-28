/*
CommandLine.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "CommandLine.h"
#include "../util/stringutils.h"
#include "../util/Log.h"
#include <sstream>
#include <fstream>

namespace sn
{

//------------------------------------------------------------------------------
CommandLine::CommandLine(int argc, char * argv[])
{
    for (s32 i = 0; i < argc; ++i)
    {
        m_args.push_back(sn::toWideString(argv[i]));
    }
}

//------------------------------------------------------------------------------
//CommandLine::CommandLine(int argc, wchar_t * argv[])
//{
//	for (u32 i = 0; i < argc; ++i)
//	{
//		m_args.push_back(argv[i]);
//	}
//}

//------------------------------------------------------------------------------
bool CommandLine::addFromFile(const std::string & filePath)
{
	std::string contents;
	if (readFile(filePath, contents))
	{
		addFromString(contents);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void CommandLine::addFromString(const std::string & str)
{
    std::istringstream iss(str, std::ios::beg);
    while (!iss.eof())
    {
        std::string arg;
        iss >> arg;
        if (!arg.empty())
        {
            m_args.push_back(sn::toWideString(arg));
        }
    }
}

//------------------------------------------------------------------------------
std::wstring CommandLine::toWideString() const
{
    if (m_args.size() == 0)
        return L"";

    std::wstringstream ss;
    ss << m_args[0];

    for (u32 i = 0; i < m_args.size(); ++i)
    {
        ss << ' ' << m_args[i];
    }

    return ss.str();
}

} // namespace sn


