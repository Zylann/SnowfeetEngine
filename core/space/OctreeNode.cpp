#include "OctreeNode.hpp"
#include "Octree.hpp"

namespace sn
{

//------------------------------------------------------------------------------
OctreeNode::OctreeNode(Octree & manager, const Vector3i & position, u32 size, u32 depth) :
    SpaceTreeNodeBase(size, depth),
    r_manager(manager),
    m_position(position)
{
    m_children[0] = nullptr;
}

//------------------------------------------------------------------------------
OctreeNode::~OctreeNode()
{
    if (m_children[0])
    {
        destroyChildren();
    }
}

// TODO [Optimize] There might be an even better approach than recursion
// The depth level of an AABB in the tree could be calculated from its integer form in one go, instead of recursing,
// because we know what is the max depth (just an idea, didn't thought which actual calculations)

//------------------------------------------------------------------------------
void OctreeNode::add(ISpacePartitionObject * obj, const IntAABB & bounds)
{
    const u32 maxDepth = r_manager.getSettings().getMaxDepth();
    if (m_depth < maxDepth)
    {
        u32 chosenChildIndex = findChildIndex(bounds);

        if (chosenChildIndex != INVALID_CHILD)
        {
            // The object fits entirely in one sub-quad, store it there
            if (m_children[0] == nullptr)
            {
                // Create children if they aren't
                createChildren();
            }
            m_children[chosenChildIndex]->add(obj, bounds);
        }
        else
        {
            // The object cannot fit in children
            m_objects.insert(obj);
        }
    }
    else
    {
        // Cannot subdivide more space, we're at max depth
        m_objects.insert(obj);
    }
}

//------------------------------------------------------------------------------
void OctreeNode::remove(ISpacePartitionObject * obj, const IntAABB & bounds)
{
    OctreeNode * node = findNode(bounds);
    SN_ASSERT(node != nullptr, "Invalid state: OctreeNode not found");
    node->m_objects.erase(obj);
    m_objects.erase(obj);
    gc();
}

//------------------------------------------------------------------------------
void OctreeNode::gc()
{
    SN_ASSERT(m_children[0] != nullptr, "Cannot gc() on leaf node");
    u32 emptyChildren = 0;
    for (u32 i = 0; i < 8; ++i)
    {
        m_children[i]->gc();
        if (m_children[i]->isEmpty())
            ++emptyChildren;
    }
    if (emptyChildren == 8)
        destroyChildren();
}

//------------------------------------------------------------------------------
void OctreeNode::createChildren()
{
    SN_ASSERT(m_children[0] == nullptr, "Children already created");

    u32 childSize = m_size / 2;
    Vector3i childPos = m_position;
    u32 childDepth = m_depth + 1;

    // TODO This is silly

    m_children[0] = new OctreeNode(r_manager, childPos, childSize, childDepth);
    ++childPos.x();
    m_children[1] = new OctreeNode(r_manager, childPos, childSize, childDepth);
    --childPos.x();
    ++childPos.y();
    m_children[2] = new OctreeNode(r_manager, childPos, childSize, childDepth);
    ++childPos.x();
    m_children[3] = new OctreeNode(r_manager, childPos, childSize, childDepth);

    --childPos.x();
    --childPos.y();
    ++childPos.z();
    m_children[8] = new OctreeNode(r_manager, childPos, childSize, childDepth);
    ++childPos.x();
    m_children[5] = new OctreeNode(r_manager, childPos, childSize, childDepth);
    --childPos.x();
    ++childPos.y();
    m_children[6] = new OctreeNode(r_manager, childPos, childSize, childDepth);
    ++childPos.x();
    m_children[7] = new OctreeNode(r_manager, childPos, childSize, childDepth);
}

//------------------------------------------------------------------------------
void OctreeNode::destroyChildren()
{
    SN_ASSERT(m_children[0] != nullptr, "Children not created");
    for (u32 i = 0; i < 8; ++i)
        delete m_children[i];
    m_children[0] = nullptr;
}

//------------------------------------------------------------------------------
u32 OctreeNode::findChildIndex(const IntAABB & bounds)
{
    u32 chosenChildIndex = INVALID_CHILD;
    s32 subSize = m_size / 2;

    // Test if bounds would fit in one sub-quad:
    // If the size fits regardless of a position
    if (bounds.width() < subSize && 
        bounds.height() < subSize && 
        bounds.depth() < subSize)
    {
        Vector3i subCenter = Vector3i(
            m_position.x() + subSize,
            m_position.y() + subSize,
            m_position.z() + subSize
        );

        // If the position doesn't makes bounds to cross an edge
        if (bounds.contains(subCenter))
        {
            // Calculate which quad to choose
            chosenChildIndex = bounds.x() - m_position.x() < subSize ? 0 : 1;
            if (bounds.y() - m_position.y() >= subSize)
                chosenChildIndex += 2;
            if (bounds.z() - m_position.z() >= subSize)
                chosenChildIndex += 4;
        }
    }

    return chosenChildIndex;
}

//------------------------------------------------------------------------------
OctreeNode * OctreeNode::findNode(const IntAABB & bounds)
{
    OctreeNode * node = this;
    u32 childIndex = node->findChildIndex(bounds);

    while (childIndex != INVALID_CHILD)
    {
        node = node->m_children[childIndex];
        childIndex = node->findChildIndex(bounds);
    }

    return node;
}

} // namespace sn

