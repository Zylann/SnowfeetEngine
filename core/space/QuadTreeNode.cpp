#include "QuadTreeNode.hpp"
#include "QuadTree.hpp"

namespace sn
{

//------------------------------------------------------------------------------
QuadTreeNode::QuadTreeNode(QuadTree & manager, const Vector2i & position, u32 size, u32 depth) :
    SpaceTreeNodeBase(size, depth),
    r_manager(manager),
    m_position(position)
{
    m_children[0] = nullptr;
}

//------------------------------------------------------------------------------
QuadTreeNode::~QuadTreeNode()
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
void QuadTreeNode::add(ISpacePartitionObject * obj, const IntRect & bounds)
{
    const u32 maxDepth = r_manager.getSettings().getMaxDepth();
    if (m_depth < maxDepth)
    {
        u32 choosenQuad = findQuadIndex(bounds);

        if (choosenQuad != INVALID_QUAD)
        {
            // The object fits entirely in one sub-quad, store it there
            if (m_children[0] == nullptr)
            {
                // Create children if they aren't
                createChildren();
            }
            m_children[choosenQuad]->add(obj, bounds);
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
void QuadTreeNode::remove(ISpacePartitionObject * obj, const IntRect & bounds)
{
    QuadTreeNode * node = findNode(bounds);
    SN_ASSERT(node != nullptr, "Invalid state: QuadTreeNode not found");
    node->m_objects.erase(obj);
    m_objects.erase(obj);
    gc();
}

//------------------------------------------------------------------------------
void QuadTreeNode::gc()
{
    SN_ASSERT(m_children[0] != nullptr, "Cannot gc() on leaf node");
    u32 emptyChildren = 0;
    for (u32 i = 0; i < 4; ++i)
    {
        m_children[i]->gc();
        if (m_children[i]->isEmpty())
            ++emptyChildren;
    }
    if (emptyChildren == 4)
        destroyChildren();
}

//------------------------------------------------------------------------------
void QuadTreeNode::createChildren()
{
    SN_ASSERT(m_children[0] == nullptr, "Children already created");
    
    u32 childSize = m_size / 2;
    Vector2i childPos = m_position;
    u32 childDepth = m_depth + 1;

    m_children[0] = new QuadTreeNode(r_manager, childPos, childSize, childDepth);
    ++childPos.x();
    m_children[1] = new QuadTreeNode(r_manager, childPos, childSize, childDepth);
    --childPos.x();
    ++childPos.y();
    m_children[2] = new QuadTreeNode(r_manager, childPos, childSize, childDepth);
    ++childPos.x();
    m_children[3] = new QuadTreeNode(r_manager, childPos, childSize, childDepth);
}

//------------------------------------------------------------------------------
void QuadTreeNode::destroyChildren()
{
    SN_ASSERT(m_children[0] != nullptr, "Children not created");
    for (u32 i = 0; i < 4; ++i)
        delete m_children[i];
    m_children[0] = nullptr;
}

//------------------------------------------------------------------------------
u32 QuadTreeNode::findQuadIndex(const IntRect & bounds)
{
    u32 choosenQuad = INVALID_QUAD;
    s32 subSize = m_size / 2;

    // Test if bounds would fit in one sub-quad:
    // If the size fits regardless of a position
    if (bounds.width() < subSize && bounds.height() < subSize)
    {
        Vector2i subCenter = Vector2i(
            m_position.x() + subSize,
            m_position.y() + subSize
        );

        // If the position doesn't makes bounds to cross an edge
        if (subCenter.x() > bounds.maxX() ||
            subCenter.x() < bounds.minX() ||
            subCenter.y() > bounds.maxY() ||
            subCenter.y() < bounds.minY())
        {
            // [2][3]
            // [0][1]

            // Calculate which quad to choose
            choosenQuad = bounds.x() - m_position.x() < subSize ? 0 : 1;
            if (bounds.y() - m_position.y() >= subSize)
                choosenQuad += 2;
        }
    }
    
    return choosenQuad;
}

//------------------------------------------------------------------------------
QuadTreeNode * QuadTreeNode::findNode(const IntRect & bounds)
{
    QuadTreeNode * node = this;
    u32 quadIndex = node->findQuadIndex(bounds);

    while (quadIndex != INVALID_QUAD)
    {
        node = node->m_children[quadIndex];
        quadIndex = node->findQuadIndex(bounds);
    }

    return node;
}

} // namespace sn

