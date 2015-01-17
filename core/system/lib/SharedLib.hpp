#ifndef __HEADER_SN_SHARED_LIB__
#define __HEADER_SN_SHARED_LIB__

#include <core/util/String.hpp>
#include <core/util/NonCopyable.hpp>

namespace sn
{

class SharedLibImpl;

class SharedLib : public NonCopyable
{
public:    
    SharedLib():
        m_impl(nullptr)
    {}

    ~SharedLib()
    {
        unload();
    }

    inline bool isLoaded() const { return m_impl != nullptr; }

    bool loadFromFile(const String & path);
    bool unload();

    void * getAddressOf(const std::string & symbolName);

private:
    SharedLibImpl * m_impl;

};

} // namespace sn

#endif // __HEADER_SN_SHARED_LIB__



