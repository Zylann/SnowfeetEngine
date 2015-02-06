/*
Log.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_LOG__
#define __HEADER_SN_LOG__

#include <core/types.hpp>
#include <core/util/NonCopyable.hpp>
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

//------------------------------------------------------------------------------
#define _SN_LOG(logType, expr) \
    do {\
        std::stringstream ss;\
        ss << expr;\
        sn::Log::get().print(logType, ss.str());\
    } while (false)

#define _SN_WLOG(logType, wexpr) \
    do {\
        std::wstringstream wss;\
        wss << wexpr;\
        sn::Log::get().wprint(logType, wss.str());\
    } while (false)

#define SN_LOG(expr)        _SN_LOG(sn::SN_LTM_INFO, expr)
#define SN_DLOG(expr)       _SN_LOG(sn::SN_LTM_DEBUG, expr)
#define SN_WARNING(expr)    _SN_LOG(sn::SN_LTM_WARNING, expr)
#define SN_ERROR(expr)      _SN_LOG(sn::SN_LTM_ERROR, expr)
#define SN_MORE(expr)       _SN_LOG(sn::SN_LTM_MORE, expr)

#define SN_WLOG(wexpr)      _SN_WLOG(sn::SN_LTM_INFO, wexpr)
#define SN_WDLOG(wexpr)     _SN_WLOG(sn::SN_LTM_DEBUG, wexpr)
#define SN_WWARNING(wexpr)  _SN_WLOG(sn::SN_LTM_WARNING, wexpr)
#define SN_WERROR(wexpr)    _SN_WLOG(sn::SN_LTM_ERROR, wexpr)
#define SN_WMORE(expr)      _SN_WLOG(sn::SN_LTM_MORE, wexpr)

//------------------------------------------------------------------------------
// Logs a message once. Subsequent executions do nothing.
// Usage: SN_LOG_ONCE(warn, "This is a warning")
// Warning: it is designed as global code, not per-object instance.
#define SN_LOG_ONCE(expr) \
    do {\
        static bool __once = true;\
        if (__once) {\
            SN_LOG(expr); \
            __once = false; \
        }\
    } while (false)

//------------------------------------------------------------------------------
namespace sn
{

enum SN_API LogTypeMask
{
    SN_LTM_DEBUG = 1,
    SN_LTM_INFO = 1 << 1,
    SN_LTM_WARNING = 1 << 2,
    SN_LTM_ERROR = 1 << 3,
    SN_LTM_ALL = SN_LTM_DEBUG | SN_LTM_INFO | SN_LTM_WARNING | SN_LTM_ERROR,
    SN_LTM_MORE = 1 << 4, // Special type meaning "continue last logging message"
    SN_LTM_NONE = 0
};

// TODO make the Log thread-safe (and threaded?)
/// \brief Simple stream wrapper providing file and/or console output
class SN_API Log : public NonCopyable
{
public:

    /// \brief Gets global Log singleton
    static Log & get();

private:

    Log() :
        m_messageType(SN_LTM_INFO),
        m_fileOutputFlags(SN_LTM_ALL),
#ifdef SN_BUILD_DEBUG
        m_consoleOutputFlags(SN_LTM_WARNING | SN_LTM_ERROR | SN_LTM_DEBUG)
#else
        m_consoleOutputFlags(SN_LTM_WARNING | SN_LTM_ERROR)
#endif
    {}

public:

    /// \brief Sets which types of messages must be written to the file output.
    /// \param flags: bitmask where set flags activate a type of output.
    /// \see Log::MessageTypeMask
    inline void setFileOutputFlags(u32 flags)
    {
        m_fileOutputFlags = flags;
    }

    /// \brief Sets which types of messages must be written to the console output.
    /// \param flags: bitmask where set flags activate a type of output.
    /// \see Log::MessageTypeMask
    inline void setConsoleOutputFlags(u32 flags)
    {
        m_consoleOutputFlags = flags;
    }

    /// \brief Sets which file will be used to output log messages.
    /// \param fpath: path to the file to open
    /// \return true if the file was successuflly open, false otherwise
    bool openFile(const std::string & fpath);

    /// \brief Closes the file used to output log messages.
    /// \note This also disables outputting messages to a file unless openFile() gets called again.
    inline void closeFile()
    {
        m_file.close();
    }

    void print(LogTypeMask logType, std::string msg);
    void wprint(LogTypeMask logType, std::wstring msg);

private:

    /// \brief File where messages can be output.
    std::ofstream m_file;

    /// \brief Type of the last message.
    LogTypeMask m_messageType;

    /// \brief Bitmask storing which messages to output to the file.
    u8 m_fileOutputFlags;

    /// \brief Bitmask storing which messages to output to the console.
    u8 m_consoleOutputFlags;

};

} // namespace sn

#endif // __HEADER_SN_LOG__

