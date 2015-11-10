#include "DriverManager.h"
#include "../util/typecheck.h"

namespace sn
{

IDriver * DriverManager::getDriver(const ObjectType & ot) const
{
    for (auto it = m_drivers.begin(); it != m_drivers.end(); ++it)
    {
        IDriver * d = it->second;
        if (d->getObjectType().is(ot))
            return d;
    }
    return nullptr;
}

void DriverManager::loadDriversFromModule(const std::string & modName)
{
    const ObjectTypeDatabase & otb = ObjectTypeDatabase::get();
    std::vector<const ObjectType*> types;
    otb.getTypesByModuleName(modName, types);

    const ObjectType & driverBaseType = getObjectType<IDriver>();

    for (auto it = types.begin(); it != types.end(); ++it)
    {
        const ObjectType & ot = **it;
        if (!ot.isAbstract() && ot.derivesFrom(driverBaseType))
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

IDriver * DriverManager::loadDriver(const ObjectType & ot)
{
    SN_LOG("Loading driver " << ot.getName() << "...");
    if (ot.derivesFrom(getObjectType<IDriver>()))
    {
        return checked_cast<IDriver*>(ot.instantiate());
    }
    else
    {
        SN_WARNING("Unrecognized driver type " << ot.getName());
        return nullptr;
    }
}

} // namespace sn

