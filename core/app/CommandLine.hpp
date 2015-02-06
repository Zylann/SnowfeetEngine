/*
CommandLine.hpp
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_COMMANDLINE__
#define __HEADER_SN_COMMANDLINE__

#include <vector>
#include <string>
#include <core/types.hpp>
#include <core/util/String.hpp>

namespace sn
{

class SN_API CommandLine
{
public:

    CommandLine(int argc, char * argv[]);
    //CommandLine(int argc, wchar_t * argv[]);

    bool addFromFile(const std::string & filePath);
    void addFromString(const std::string & str);

    inline u32 getArgCount() const { return m_args.size(); }
    inline const String & getArg(u32 i) const { return m_args[i]; }

    std::wstring toWideString() const;

private:

    std::vector<String> m_args;

};

} // namespace sn

#endif



