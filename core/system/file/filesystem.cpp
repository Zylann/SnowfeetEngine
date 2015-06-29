#include "filesystem.h"

namespace sn
{

bool getFilesRecursively(String topDirectory, std::vector<FileNode> & out_nodes)
{
    std::vector<FileNode> nodes;
    if (!getFiles(topDirectory, nodes))
        return false;

    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        FileNode & node = *it;
        node.path = FilePath::join(topDirectory, node.path);
        if (node.isDirectory)
        {
            if (!getFilesRecursively(node.path, out_nodes))
                return false;
        }
        else
        {
            out_nodes.push_back(node);
        }
    }

    return true;
}

} // namespace

