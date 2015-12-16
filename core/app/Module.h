#ifndef __HEADER_SN_MODULE__
#define __HEADER_SN_MODULE__

#include <core/app/ModuleArgs.h>
#include <core/system/SharedLib.h>
#include <core/util/RefCounted.h>

namespace sn
{

class Module : public RefCounted
{
public:
    static Module * loadFromFile(const std::string & filePath);

    const std::string & getName() const;

    bool entryPoint(ModuleLoadArgs args);
    bool exitPoint(ModuleUnloadArgs args);

protected:
    virtual ~Module();

private:
    Module();

private:
    SharedLib * m_library;
    NativeModLoadFunc m_entryPoint;
    NativeModUnloadFunc m_exitPoint;
    std::string m_name;

};

} // namespace sn

#endif // __HEADER_SN_MODULE__

