#ifndef __HEADER_SN_DRIVERMANAGER__
#define __HEADER_SN_DRIVERMANAGER__

#include <core/drivers/VideoDriver.hpp>

namespace sn
{

class SN_API DriverManager
{
public:

    DriverManager() :
        m_videoDriver(nullptr)
    {}

    void loadDriversFromModule(const std::string & modName);
    void unloadDriversFromModule(const std::string & modName);
    void unloadAllDrivers();

    IVideoDriver * getVideoDriver() const { return m_videoDriver; }

private:
    IDriver * loadDriver(const ObjectType & ot);

private:
    std::unordered_map<std::string,IDriver*> m_drivers;
    IVideoDriver * m_videoDriver;

};

} // namespace sn

#endif // __HEADER_SN_DRIVERMANAGER__


