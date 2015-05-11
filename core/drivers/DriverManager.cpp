#include "DriverManager.hpp"
#include "../util/typecheck.hpp"

namespace sn
{

void DriverManager::loadDriversFromModule(const std::string & modName)
{
    const ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    std::vector<const ObjectType*> types;
    otb.getTypesByModuleName(modName, types);

    const ObjectType & driverBaseType = getObjectType<IDriver>();

    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & ot = **it;
        if (ot.derivesFrom(driverBaseType))
        {
            IDriver * d = loadDriver(ot);
            if (d)
            {
                m_drivers[ot.getName()] = d;
            }
        }
    }
}

void DriverManager::unloadDriversFromModule(const std::string & modName)
{
    auto drivers = m_drivers;
    for (auto it = drivers.begin(); it != drivers.end(); ++it)
    {
        IDriver * d = it->second;
        const ObjectType & ot = d->getObjectType();
        if (ot.getModuleName() == modName)
        {
            drivers.erase(ot.getName());
            delete d;
        }
    }
}

void DriverManager::unloadAllDrivers()
{
    for (auto it = m_drivers.begin(); it != m_drivers.end(); ++it)
    {
        IDriver * d = it->second;
        delete d;
    }
    m_drivers.clear();
}

namespace
{
    template <class Driver_T>
    Driver_T * instantiateDriver(Driver_T *& member, const ObjectType & ot)
    {
        if (member)
            SN_ERROR("Video driver already installed (" << member->getObjectType().getName() << ")");
        else
            member = checked_cast<Driver_T*>(ot.instantiate());
        return member;
    }
}

IDriver * DriverManager::loadDriver(const ObjectType & ot)
{
    SN_LOG("Loading driver " << ot.getName() << "...");
    if (ot.derivesFrom(getObjectType<IVideoDriver>()))
    {
        return instantiateDriver(m_videoDriver, ot);
    }
    else
    {
        SN_WARNING("Unrecognized driver type " << ot.getName());
    }
}

} // namespace sn

