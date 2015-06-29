#ifndef __HEADER_SN_SERIALIZATIONCONTEXT__
#define __HEADER_SN_SERIALIZATIONCONTEXT__

#include <string>

namespace sn
{

class SN_API SerializationContext
{
public:
    SerializationContext(const std::string & moduleName) :
        m_module(moduleName)
    {}

    /// \brief In which module the data is currently serialized
    const std::string & getModule() const { return m_module; }

private:
    std::string m_module;
};

} // namespace sn

#endif // __HEADER_SN_SERIALIZATIONCONTEXT__

