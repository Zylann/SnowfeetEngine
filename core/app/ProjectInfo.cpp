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
const char * ProjectInfo::FILE_NAME = "project.json";

//------------------------------------------------------------------------------
bool ProjectInfo::loadFromFile(const String & pathToProjects, const String & pathToFile)
{
    // Reset info
    filePath = L"";
    dependencies.clear();
    modules.clear();
    directory = L"";

    filePath = pathToFile;
    directory = getFileFolder(filePath);
    SN_ASSERT(!directory.empty(), "A project file must always be loaded from a directory");

    // The name is the folder
    size_t sepIndex = directory.find_last_of('/');
    if (sepIndex == std::string::npos)
        sepIndex = directory.find_last_of('\\');
    if (sepIndex == std::string::npos)
        name = toString(directory);
    else
        name = toString(directory.substr(sepIndex+1));
    //name = getFileNameWithoutExtension(toString(filePath));

    // Get full path
    String fullPath = pathToProjects + L'/' + filePath;

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
                    SN_WARNING("Expected service name, got arguments in " + toString(filePath));
                }
            }
        }
    }
}

} // namespace sn

