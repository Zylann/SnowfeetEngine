/*
SharedLib.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_SHARED_LIB__
#define __HEADER_SN_SHARED_LIB__

#include <core/util/String.h>
#include <core/util/NonCopyable.h>

namespace sn
{

class SharedLibImpl;

/// \brief Platform-independant interface to load dynamic libraries.
class SharedLib : public NonCopyable
{
public:
    SharedLib();
    ~SharedLib();

    /// \brief Tells if the library is loaded
    inline bool isLoaded() const { return m_impl != nullptr; }

    /// \brief Loads the library from a path to its file (.dll, .so etc)
    /// \return True on success, false on failure.
    bool loadFromFile(const String & path);

    /// \brief Unloads the library.
    /// \warning Make sure no native objects from this library are still used somewhere,
    /// or loss of matter will occur.
    /// \return True on success, false on failure.
    bool unload();

    /// \brief Gets the address of a symbol in the library from its name.
    /// The symbol must be visible (exported) and not name-mangled.
    void * getAddressOf(const std::string & symbolName);

private:
    SharedLibImpl * m_impl;

};

} // namespace sn

#endif // __HEADER_SN_SHARED_LIB__



