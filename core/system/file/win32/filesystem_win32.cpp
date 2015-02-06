/*
filesystem_win32.cpp
Copyright (C) 2012-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../filesystem.hpp"
#include "../../../util/Log.hpp"
#include <Windows.h>

namespace sn
{

//------------------------------------------------------------------------------
bool pathExists(String path)
{
    return (GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES);
}

//------------------------------------------------------------------------------
bool makeDir(String path)
{
    if (path.empty())
        return false;
    BOOL success = CreateDirectoryW(path.c_str(), NULL);
    if (success)
        return true;
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return true;
    else
        SN_WERROR("makeDir: can't create folder \"" << path << '"');
    return false;
}

//------------------------------------------------------------------------------
bool getFiles(String topDirectory, std::vector<FileNode> & out_nodes)
{
    std::string nodeName, fileName;
    bool searchResult;
    bool success = false;

    //content.dirPath = dirPath;
    String path = topDirectory + L"\\*.*";
    WIN32_FIND_DATAW findData;
    HANDLE l_searchHandle = FindFirstFileW(path.c_str(), &findData);

    if (l_searchHandle == INVALID_HANDLE_VALUE)
    {
        SN_WERROR("getFiles: INVALID_HANDLE_VALUE for path \"" << path << '"');
    }
    else
    {
        while ((searchResult = FindNextFileW(l_searchHandle, &findData)) != 0)
        {
            bool isDirectory = false;
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                isDirectory = true;

            String fileName = findData.cFileName;
            // not include '.' and '..' folders (current and parent)
            if (fileName != L"." && fileName != L"..")
            {
                out_nodes.push_back(FileNode(fileName, isDirectory));
            }
        }

        success = true;
    }

    FindClose(l_searchHandle);
    return success;
}

} // namespace sn

