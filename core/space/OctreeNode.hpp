#ifndef __HEADER_SN_SPACE_OCTREENODE__
#define __HEADER_SN_SPACE_OCTREENODE__

#include <core/space/SpaceTreeNodeBase.hpp>
#include <core/util/NonCopyable.hpp>
#include <core/math/Vector3.hpp>
#include <unordered_set>

namespace sn
{

class Octree;

/// \brief Internal data structure used by QuadTree.
/// Works with integer coordinates for better efficiency.
class OctreeNode : public SpaceTreeNodeBase
{
public:
    OctreeNode(Octree & manager, const Vector3i & position, u32 size, u32 depth);
    ~OctreeNode();

    /// \brief Adds an object to this node.
    /// Assumes that the object's bounds are entirely within the node.
    void add(ISpacePartitionObject * obj, const IntAABB & bounds);

    /// \brief Removes an object from this node.
    /// Assumes that the object's bounds are entirely within the node.
    void remove(ISpacePartitionObject * obj, const IntAABB & bounds);

    inline bool isLeaf() { return m_children[0] == nullptr; }
    inline bool isEmpty() { return m_objects.empty() && m_children[0] == nullptr; }

    inline const Vector3i & getPosition() const { return m_position; }

    /// \brief recursively deallocates sub-nodes if no objects are stored in them
    void gc();

private:
    void createChildren();
    void destroyChildren();

    /// \brief Finds which child node would entirely contain given bounds.
    /// Assumes that the object's bounds are entirely within the current node.
    /// Child nodes allocation is not needed.
    /// \return Index of matching child node, INVALID_CHILD if none match.
    u32 findChildIndex(const IntAABB & bounds);

    OctreeNode * findNode(const IntAABB & bounds);

private:
    static const u32 INVALID_CHILD = 8;

    Octree & r_manager;

    // [0] = (0,0,0)
    // [1] = (1,0,0)
    // [2] = (0,1,0)
    // [3] = (1,1,0)
    // [4] = (0,0,1)
    // [5] = (1,0,1)
    // [6] = (0,1,1)
    // [7] = (1,1,1)
    // There can't be null and non-null children at a time.
    OctreeNode * m_children[8];

    Vector3i m_position;
};

} // namespace sn

#endif // __HEADER_SN_SPACE_OCTREENODE__


