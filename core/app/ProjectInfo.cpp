/*
ModuleInfo.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "ProjectInfo.h"
#include <core/sml/variant_serialize.h>
#include "../sml/SmlParser.h"
#include "../util/stringutils.h"
#include "../util/Log.h"
#include <fstream>

namespace sn
{
	namespace
	{
		void unserialize(const Variant & in, std::vector<String> & out)
		{
			if (in.isArray())
			{
				const Variant::Array & a = in.getArray();
				for (u32 i = 0; i < a.size(); ++i)
				{
					const Variant & elem = a[i];
					if (elem.isString())
					{
						std::string str = elem.getString();
						out.push_back(toWideString(str));
					}
				}
			}
		}
    }

//------------------------------------------------------------------------------
bool ProjectInfo::loadFromFile(const String & pathToProjects, const String & modPath)
{
    // Reset info
    modFilePath = L"";
    dependencies.clear();
    modules.clear();
    directory = L"";

    modFilePath = modPath;
    directory = getFileFolder(modFilePath);
    name = getFileNameWithoutExtension(toString(modFilePath));

    // Get full path
    String fullPath = pathToProjects + L'/' + modPath;

    // Parse module
    Variant v;
    std::ifstream ifs(toString(fullPath), std::ios::in | std::ios::binary);
    if (!ifs.good())
    {
        return false;
    }
	SmlParser parser;
    parser.parseValue(ifs, v);
    ifs.close();

    return setDataFromVariant(v);
}

//------------------------------------------------------------------------------
bool ProjectInfo::setDataFromVariant(const Variant & v)
{
	if (!v.isDictionary())
		return false;

    // Get namespace
    scriptNamespace = v["namespace"].getString();

    // Get startup scene if any
    startupScene = v["startupScene"].getString();

    // Get services if any
    parseServices(v["services"]);

    // Get bindings
	sn::unserialize(v["modules"], modules);

    // Get dependencies
	unserialize(v["dependencies"], dependencies);

    // Get update layers
    updateLayers = v["updateLayers"];

    return true;
}

//------------------------------------------------------------------------------
void ProjectInfo::parseServices(const Variant & o)
{
    if (o.isArray())
    {
        const Variant::Array & a = o.getArray();
        for (u32 i = 0; i < a.size(); ++i)
        {
            const Variant & serviceValue = o[i];
            if (serviceValue.isString())
            {
                Service s;
                s.type = serviceValue.getString();
                services.push_back(s);
            }
            else if (serviceValue.isDictionary())
            {
                if (services.size() >= 1)
                {
                    services.back().args = serviceValue;
                }
                else
                {
                    SN_WARNING("Expected service name, got arguments in " + toString(modFilePath));
                }
            }
        }
    }
}

} // namespace sn

