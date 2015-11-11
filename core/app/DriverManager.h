#ifndef __HEADER_SN_DRIVERMANAGER__
#define __HEADER_SN_DRIVERMANAGER__

#include <core/util/typecheck.h>
#include <core/app/Driver.h>

namespace sn
{

class SN_API DriverManager
{
public:

    void loadDriversFromModule(const std::string & modName);
    void unloadDriversFromModule(const std::string & modName);
    void unloadAllDrivers();

    template <class Driver_T>
    Driver_T * getDriver() const
    {
        return checked_cast<Driver_T*>(getDriver(sn::getObjectType<Driver_T>()));
    }

    IDriver * getDriver(const ObjectType & ot) const;

private:
    IDriver * loadDriver(const ObjectType & ot);

private:
    std::unordered_map<std::string,IDriver*> m_drivers;

};

} // namespace sn

#endif // __HEADER_SN_DRIVERMANAGER__


