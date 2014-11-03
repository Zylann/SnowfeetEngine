/*
exception.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetEngine.
*/

#ifndef __HEADER_SN_EXCEPTION__
#define __HEADER_SN_EXCEPTION__

#include <string>
#include <exception>
#include <core/config.hpp>

// Assertion-like call that raises an exception instead of exit the program.
// Usage: SN_XASSERT(someValue != 0, "message");
#define SN_XASSERT(predicate, msg) \
    do{\
        if (!predicate) {\
            std::stringstream ss; \
            ss << __FILE__ << ", line " << __LINE__ << ": " << #predicate << "; " << msg; \
            throw sn::Exception(ss.str()); \
        }\
    } while (false)

#define SN_EXCEPTION(msg)\
    do{\
        std::stringstream ss;\
        ss << __FILE__ << ", line " << __LINE__ << ": " << msg; \
        throw Exception(ss.str());\
    } while (false)

namespace sn
{

class Exception : public std::exception
{
public:
    Exception(const std::string msg)
    {
        m_msg = msg;
    }

    const char * what() const SN_NOEXCEPT override { return m_msg.c_str(); }

private:
    std::string m_msg;
};

} // namespace sn

#endif // __HEADER_SN_EXCEPTION__


