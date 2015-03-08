/*
filesystem.cpp
Copyright (C) 2012-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_FILESYSTEM__
#define __HEADER_SN_FILESYSTEM__

#include <core/util/String.hpp>
#include <vector>
#include <core/system/file/FilePath.hpp>

namespace sn
{

/// \brief Represents a file descriptor in file system functions.
/// (Not to be mixed with actual file read/write)
struct SN_API FileNode
{
    FileNode(String path_, bool isDirectory_) :
        path(path_),
        isDirectory(isDirectory_)
    {}

    String path;
    bool isDirectory;
};

// Platform-specific

/// \brief Tests if the given path exists (wether it's to a file or folder)
/// \return true if the path exists, false otherwise
bool SN_API pathExists(String path);

/// \brief Creates a directory at the given path, if the parent directory exists.
/// \return true on success, false on error 
bool SN_API makeDir(String path);

/// \brief Gets the list of files and directories within a directory (not recursive)
/// \param topDirectory: directory from which to list the files
/// \param out_nodes: returned list of file nodes
/// \return true on success, false on error
bool SN_API getFiles(String topDirectory, std::vector<FileNode> & out_nodes);

// Platform-independent

/// \brief Recursively gets the list of files within a directory.
/// \param topDirectory: directory from which to list the files
/// \param out_nodes: returned list of file nodes, in arbitrary order. No directories.
/// \return true on success, false on error
bool SN_API getFilesRecursively(String topDirectory, std::vector<FileNode> & out_nodes);

} // namespace sn

#endif // __HEADER_SN_FILESYSTEM__



