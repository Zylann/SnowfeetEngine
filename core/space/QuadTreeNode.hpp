#ifndef __HEADER_SN_SPACE_QUADTREENODE__
#define __HEADER_SN_SPACE_QUADTREENODE__

#include <core/space/SpaceTreeNodeBase.hpp>
#include <core/util/NonCopyable.hpp>
#include <core/math/Vector2.hpp>
#include <unordered_set>

namespace sn
{

class QuadTree;

/// \brief Internal data structure used by QuadTree.
/// Works with integer coordinates for better efficiency.
class QuadTreeNode : public SpaceTreeNodeBase
{
public:
    static const u32 INVALID_QUAD = 4;

    QuadTreeNode(QuadTree & manager, const Vector2i & position, u32 size, u32 depth);
    ~QuadTreeNode();

    /// \brief Adds an object to this node.
    /// Assumes that the object's bounds are entirely within the node.
    void add(ISpacePartitionObject * obj, const IntRect & bounds);

    /// \brief Removes an object from this node.
    /// Assumes that the object's bounds are entirely within the node.
    void remove(ISpacePartitionObject * obj, const IntRect & bounds);

    inline bool isLeaf() { return m_children[0] == nullptr; }
    inline bool isEmpty() { return m_objects.empty() && m_children[0] == nullptr; }

    inline const Vector2i & getPosition() const { return m_position; }

    /// \brief recursively deallocates sub-nodes if no objects are stored in them
    void gc();

private:
    void createChildren();
    void destroyChildren();

    /// \brief Finds which child node would entirely contain given bounds.
    /// Assumes that the object's bounds are entirely within the current node.
    /// Child nodes allocation is not needed.
    /// \return Index of matching child node, INVALID_QUAD if none match.
    u32 findQuadIndex(const IntRect & bounds);

    QuadTreeNode * findNode(const IntRect & bounds);

private:
    QuadTree & r_manager;

    // [2][3]
    // [0][1]
    // There can't be null and non-null children at a time.
    QuadTreeNode * m_children[4];

    Vector2i m_position;
};

} // namespace sn

#endif // __HEADER_SN_SPACE_QUADTREENODE__


