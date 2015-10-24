#include <core/app/ModuleArgs.h>
#include <core/asset/AssetDatabase.h>

#include "mod_SystemDialog.h"
#include "bind/sq_systemdialog.h"


int loadSnowfeetModule_ModSystemDialog(ModuleLoadArgs args)
{
	using namespace systemdialog;

    // auto & otb = *args.objectTypeDatabase;

	HSQUIRRELVM vm = args.squirrelVM;

	bindSystemDialog(vm);

    return 0;
}

int unloadSnowfeetModule_ModSystemDialog(ModuleUnloadArgs args)
{
    return 0;
}


