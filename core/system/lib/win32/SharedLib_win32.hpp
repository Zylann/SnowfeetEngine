#ifndef __HEADER_SN_SHAREDLIB_WIN32__
#define __HEADER_SN_SHAREDLIB_WIN32__

#include <core/util/String.hpp>
#include <Windows.h>

namespace sn
{

class SharedLibImpl
{
public:
    SharedLibImpl(const String & filePath);
    ~SharedLibImpl();

    inline bool isLoaded() const { return m_handle != nullptr; }
    void * getAddressOf(const std::string & name);

private:
    HINSTANCE m_handle;
};

} // namespace sn

#endif // __HEADER_SN_SHAREDLIB_WIN32__


