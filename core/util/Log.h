/*
Log.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_LOG__
#define __HEADER_SN_LOG__

#include <core/types.h>
#include <core/util/NonCopyable.h>
#include <core/system/Lock.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

//------------------------------------------------------------------------------
#define _SN_LOG(logType, expr) \
    do {\
        std::stringstream __sn_ss;\
        __sn_ss << expr;\
        sn::Log::get().print(logType, __sn_ss.str());\
    } while (false)

#define _SN_WLOG(logType, wexpr) \
    do {\
        std::wstringstream __sn_wss;\
        __sn_wss << wexpr;\
        sn::Log::get().wprint(logType, __sn_wss.str());\
    } while (false)

#ifdef SN_BUILD_DEBUG
#define _SN_LOG_PREFIX __FILE__ << ":" << __LINE__ << ": " << 
#else
#define _SN_LOG_PREFIX
#endif

#define SN_LOG(expr)        _SN_LOG(sn::SN_LTM_INFO, expr)
#define SN_DLOG(expr)       _SN_LOG(sn::SN_LTM_DEBUG, expr)
#define SN_WARNING(expr)    _SN_LOG(sn::SN_LTM_WARNING, expr)
#define SN_ERROR(expr)      _SN_LOG(sn::SN_LTM_ERROR, _SN_LOG_PREFIX expr)
#define SN_MORE(expr)       _SN_LOG(sn::SN_LTM_MORE, expr)

#define SN_WLOG(wexpr)      _SN_WLOG(sn::SN_LTM_INFO, wexpr)
#define SN_WDLOG(wexpr)     _SN_WLOG(sn::SN_LTM_DEBUG, wexpr)
#define SN_WWARNING(wexpr)  _SN_WLOG(sn::SN_LTM_WARNING, wexpr)
#define SN_WERROR(wexpr)    _SN_WLOG(sn::SN_LTM_ERROR, _SN_LOG_PREFIX wexpr)
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

/// \brief Simple stream wrapper providing file and/or console output.
/// This class can be used from multiple threads.
class SN_API Log : public NonCopyable
{
public:

    /// \brief Gets global Log singleton
    static Log & get();

public:

    /// \brief Sets which types of messages must be written to the file output.
    /// \param flags: bitmask where set flags activate a type of output.
    /// \see Log::MessageTypeMask
    void setFileOutputFlags(u32 flags);

    /// \brief Sets which types of messages must be written to the console output.
    /// \param flags: bitmask where set flags activate a type of output.
    /// \see Log::MessageTypeMask
    inline void setConsoleOutputFlags(u32 flags);

    /// \brief Sets which file will be used to output log messages.
    /// \param fpath: path to the file to open
    /// \return true if the file was successuflly open, false otherwise
    bool openFile(const std::string & fpath);

    /// \brief Closes the file used to output log messages.
    /// \note This also disables outputting messages to a file unless openFile() gets called again.
    void closeFile();

    /// \brief Pushes a message to the log system.
    /// \param logType: nature of the message
    /// \param msg: message string
    void print(LogTypeMask logType, std::string msg);

    /// \brief Pushes a message to the log system.
    /// \param logType: nature of the message
    /// \param msg: message string (wide characters)
    void wprint(LogTypeMask logType, std::wstring msg);

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

private:

    /// \brief File where messages can be output.
    std::ofstream m_file;

    /// \brief Type of the last message.
    LogTypeMask m_messageType;

    /// \brief Bitmask storing which messages to output to the file.
    u8 m_fileOutputFlags;

    /// \brief Bitmask storing which messages to output to the console.
    u8 m_consoleOutputFlags;

    /// \brief Synchronization mutex to be able to use the logger from multiple threads
    Mutex m_mutex;

};

} // namespace sn

#endif // __HEADER_SN_LOG__

