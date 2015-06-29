/*
Log.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Log.h"
#include "stringutils.h"
#include "../system/console/console.h"

#ifdef _MSC_VER
#include <Windows.h> // For OutputDebugString on Windows
#endif

namespace sn
{

//------------------------------------------------------------------------------
// Static
Log & Log::get()
{
    static Log s_log;
    return s_log;
}

//------------------------------------------------------------------------------
void Log::setFileOutputFlags(u32 flags)
{
    Lock lock(m_mutex);
    m_fileOutputFlags = flags;
}

//------------------------------------------------------------------------------
void Log::setConsoleOutputFlags(u32 flags)
{
    Lock lock(m_mutex);
    m_consoleOutputFlags = flags;
}

//------------------------------------------------------------------------------
void Log::closeFile()
{
    Lock lock(m_mutex);
    m_file.close();
}

//------------------------------------------------------------------------------
bool Log::openFile(const std::string & fpath)
{
    Lock lock(m_mutex);
    m_file.open(fpath.c_str());
    if(!m_file.good())
    {
        std::cout << "E: Log: failed to open file \"" << fpath << '"' << std::endl;
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
void Log::print(LogTypeMask logType, std::string msg)
{
    std::string prefix;

    switch (logType)
    {
    case SN_LTM_INFO:     prefix = "I: "; break;
    case SN_LTM_WARNING:  prefix = "W: "; break;
    case SN_LTM_ERROR:    prefix = "E: "; break;
    case SN_LTM_MORE:     prefix = " | "; break;
    default:              prefix = "D: "; break; // SN_LTM_DEBUG
    }

    Lock lock(m_mutex);

    // TODO Log: add timestamp

    // Set console color
    ConsoleColor oldColor = getConsoleForeground();
    ConsoleColor currentColor = oldColor;
    switch (logType)
    {
    case SN_LTM_INFO:     currentColor = SN_CC_DEFAULT; break;
    case SN_LTM_WARNING:  currentColor = SN_CC_YELLOW; break;
    case SN_LTM_ERROR:    currentColor = SN_CC_RED; break;
    default:              currentColor = SN_CC_DARKGRAY; break; // SN_LTM_DEBUG
    }
    if (currentColor != oldColor)
    {
        setConsoleForeground(currentColor);
    }

    msg = prefix + msg;

    if (m_file && (m_messageType & m_fileOutputFlags))
    {
        // Write message in log file
        m_file << msg << std::endl;
    }

    if (m_consoleOutputFlags & m_consoleOutputFlags)
    {
        // Write message in console
        if (m_messageType == SN_LTM_ERROR)
        {
            std::cerr << msg << std::endl;
        }
        else
        {
            std::cout << msg << std::endl;
        }
    }

    // Restore old console color
    if (currentColor != oldColor)
    {
        setConsoleForeground(oldColor);
    }

#if defined(SN_BUILD_DEBUG) && defined(_MSC_VER)
    msg += '\n';
    OutputDebugString(msg.c_str());
#endif
}

//------------------------------------------------------------------------------
void Log::wprint(LogTypeMask logType, std::wstring msg)
{
    // TODO Log: better support for wide chars
    print(logType, sn::toString(msg));
}


} // namespace sn

