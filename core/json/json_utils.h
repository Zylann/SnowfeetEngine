/*
json_utils.h
Copyright (C) 2010-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_JSON_UTILS__
#define __HEADER_SN_JSON_UTILS__

#include <JsonBox.h>
#include <core/types.h>

// Json serialization helpers for various composed types

namespace sn
{

//------------------------------------------------------------------------------
/// \brief Utility function that loads a JSON file, prints messages and checks for a version member in the document.
/// \param document: the JSON data tree to load
/// \param filePath: path to the JSON file
/// \param checkVersion: the version to check in the file. If -1, no check will be made. If >= 0, a check will be made,
/// and an error will be generated if it doesn't matches.
/// \param openError: should the function generate an error message if the file can't be open?
/// \return false if the file could'nt be loaded or if the version didn't matched, true if success.
bool loadFromFile(JsonBox::Value & document, const std::string & filePath, s32 checkVersion=-1, bool openError=true);

bool saveToFile(JsonBox::Value & document, const std::string & filePath);

} // namespace sn

#endif // __HEADER_SN_JSON_UTILS__


