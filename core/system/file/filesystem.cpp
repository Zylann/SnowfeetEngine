#include "filesystem.hpp"

namespace sn
{

bool getFilesRecursively(String topDirectory, std::vector<FileNode> & out_nodes)
{
    std::vector<FileNode> nodes;
    getFiles(topDirectory, nodes);

    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        const FileNode & node = *it;
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

