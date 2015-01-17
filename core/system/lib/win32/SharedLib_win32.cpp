#include "../SharedLib.hpp"
#include "SharedLib_win32.hpp"

namespace sn
{

SharedLibImpl::SharedLibImpl(const String & filePath)
{
    // Load the shared library
    HINSTANCE hLib = LoadLibraryW(filePath.c_str());
    m_handle = hLib;
}

SharedLibImpl::~SharedLibImpl()
{
    if (m_handle)
        FreeLibrary(m_handle);
}

void * SharedLibImpl::getAddressOf(const std::string & name)
{
    return GetProcAddress(m_handle, name.c_str());
}

//------------------------------------------------------------------------------
// SharedLib
//------------------------------------------------------------------------------

bool SharedLib::loadFromFile(const String & path)
{
    if (isLoaded())
        return true;
    m_impl = new SharedLibImpl(path);
    if (!m_impl->isLoaded())
        unload();
    return isLoaded();
}

bool SharedLib::unload()
{
    if (m_impl)
    {
        delete m_impl;
        m_impl = nullptr;
    }
    return !isLoaded();
}

void * SharedLib::getAddressOf(const std::string & symbolName)
{
    if (m_impl)
        return m_impl->getAddressOf(symbolName);
    else
        return nullptr;
}

} // namespace sn


