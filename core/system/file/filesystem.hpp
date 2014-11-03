#ifndef __HEADER_SN_FILESYSTEM__
#define __HEADER_SN_FILESYSTEM__

#include <core/util/String.hpp>
#include <vector>

namespace sn
{

struct FileNode
{
    FileNode(String path_, bool isDirectory_) :
        path(path_),
        isDirectory(isDirectory_)
    {}

    String path;
    bool isDirectory;
};

bool pathExists(String path);
bool makeDir(String path);
bool getFiles(String topDirectory, std::vector<FileNode> & out_nodes);

} // namespace sn

#endif // __HEADER_SN_FILESYSTEM__



