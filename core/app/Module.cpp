#include "Module.h"

namespace sn
{

Module::Module() : RefCounted(),
    m_library(nullptr),
    m_entryPoint(nullptr),
    m_exitPoint(nullptr)
{
}

Module::~Module()
{
    if (m_library)
        delete m_library;
}

const std::string & Module::getName() const
{
    return m_name;
}

// Static
Module * Module::loadFromFile(const std::string & filePath)
{
    SharedLib * lib = new SharedLib();
    if (lib->loadFromFile(sn::toWideString(filePath)))
    {
        std::string name = sn::getFileNameWithoutExtension(filePath);

        std::string loadFuncName = sn::getLoadFuncName(name);
        std::string unloadFuncName = sn::getUnloadFuncName(name);

        NativeModLoadFunc loadFunc = (NativeModLoadFunc)(lib->getAddressOf(loadFuncName));
        NativeModUnloadFunc unloadFunc = (NativeModLoadFunc)(lib->getAddressOf(unloadFuncName));

        if (loadFunc && unloadFunc)
        {
            Module * mod = new Module();
            mod->m_library = lib;
            mod->m_entryPoint = loadFunc;
            mod->m_exitPoint = unloadFunc;
            mod->m_name = name;
            return mod;
        }
        else
        {
            SN_ERROR("Entry points not found for library \"" << filePath << "\"");
        }
    }
    else
    {
        SN_ERROR("Unable to load library \"" << filePath << "\"");
    }

    delete lib;
    return nullptr;
}

bool Module::entryPoint(ModuleLoadArgs args)
{
    // Execute entry point
    int retval = m_entryPoint(args);
    if (retval < 0)
    {
        SN_ERROR("Module loading returned error code " << retval);
        return false;
    }
    return true;
}

bool Module::exitPoint(ModuleUnloadArgs args)
{
    // Execute exit point
    int unloadResult = m_exitPoint(args);
    if (unloadResult < 0)
    {
        SN_ERROR("Module unloading returned error code " << unloadResult);
        return false;
    }
    return true;
}

} // namespace sn

