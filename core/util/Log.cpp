#include "Log.hpp"
#include "stringutils.hpp"

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
bool Log::openFile(const std::string & fpath)
{
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

    // TODO Log: add timestamp

    msg = prefix + msg;

    if (m_file && (m_messageType & m_fileOutputFlags))
    {
        m_file << msg << std::endl;
    }

    if (m_consoleOutputFlags & m_consoleOutputFlags)
    {
        if (m_messageType == SN_LTM_ERROR)
        {
            std::cerr << msg << std::endl;
        }
        else
        {
            std::cout << msg << std::endl;
        }
    }

#if defined(SN_DEBUG) && defined(_MSC_VER)
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

