#ifndef __HEADER_SN_SPACE_NTREENODE__
#define __HEADER_SN_SPACE_NTREENODE__

#include <core/space/SpaceTreeNodeBase.hpp>
#include <core/space/SpaceTreeSettings.hpp>

namespace sn
{

template <unsigned int D>
class NTreeNode : public SpaceTreeNodeBase
{
public:
    //--------------------------
    static const u32 CHILD_COUNT = 1 << D;
    static const u32 INVALID_CHILD = CHILD_COUNT;

    //--------------------------
    NTreeNode(SpaceTreeSettings & settingsRef, Vector<s32, D> position, u32 size, u32 depth) :
        SpaceTreeNodeBase(size, depth),
        r_settings(settingsRef),
        m_position(position)
    {
        SN_ASSERT(D <= 3, "Dimensions above 3 aren't supported");
        m_children[0] = nullptr;
    }

    //--------------------------
    ~NTreeNode()
    {
        if (m_children[0])
        {
            destroyChildren();
        }
    }

    //--------------------------
    /// \brief Adds an object to this node.
    /// Assumes that the object's bounds are entirely within the node.
    void add(ISpacePartitionObject * obj, const Area<s32, D> & bounds)
    {
        const u32 maxDepth = r_settings.getMaxDepth();
        if (m_depth < maxDepth)
        {
            u32 choosenChildIndex = findChildIndex(bounds);

            if (choosenChildIndex != INVALID_CHILD)
            {
                // The object fits entirely in one sub-quad, store it there
                if (m_children[0] == nullptr)
                {
                    // Create children if they aren't
                    createChildren();
                }
                m_children[choosenChildIndex]->add(obj, bounds);
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

    //--------------------------
    /// \brief Removes an object from this node.
    /// Assumes that the object's bounds are entirely within the node.
    void remove(ISpacePartitionObject * obj, const Area<s32, D> & bounds)
    {
        NTreeNode * node = findNode(bounds);
        SN_ASSERT(node != nullptr, "Invalid state: QuadTreeNode not found");
        node->m_objects.erase(obj);
        m_objects.erase(obj);
        gc();
    }

    //--------------------------
    inline bool isLeaf() { return m_children[0] == nullptr; }

    //--------------------------
    inline bool isEmpty() { return m_objects.empty() && m_children[0] == nullptr; }

    //--------------------------
    inline const Vector<s32, D> getPosition() const { return m_position; }

    //--------------------------
    /// \brief recursively deallocates sub-nodes if no objects are stored in them
    void gc()
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

private:
    //--------------------------
    void createChildren()
    {
        SN_ASSERT(m_children[0] == nullptr, "Children already created");

        u32 childSize = m_size / 2;
        Vector<s32, D> childPos = m_position;
        u32 childDepth = m_depth + 1;

        for (u32 i = 0; i < CHILD_COUNT; ++i)
        {
            for (u32 d = 0; d < D; ++d)
            {
                // TODO Use a lookup table?
                childPos[d] = m_position[d] + (((1 << d) & i) >> d);
            }

            m_children[i] = new NTreeNode(r_settings, childPos, childSize, childDepth);
        }
    }

    //--------------------------
    void destroyChildren()
    {
        SN_ASSERT(m_children[0] != nullptr, "Children not created");
        for (u32 i = 0; i < 4; ++i)
            delete m_children[i];
        m_children[0] = nullptr;
    }

    //--------------------------
    /// \brief Finds which child node would entirely contain given bounds.
    /// Assumes that the object's bounds are entirely within the current node.
    /// Child nodes allocation is not needed.
    /// \return Index of matching child node, INVALID_CHILD if none match.
    u32 findChildIndex(const Area<s32, 2> & bounds)
    {
        u32 choosenChild = INVALID_CHILD;
        s32 subSize = m_size / 2;

        // Test if bounds would fit in one sub-quad:
        // If the size fits regardless of a position
        if (bounds.width() < subSize &&
            bounds.height() < subSize)
        {
            Vector<s32,2> subCenter(
                m_position.x() + subSize,
                m_position.y() + subSize
            );

            // If the position doesn't makes bounds to cross an edge
            if (bounds.contains(subCenter()))
            {
                // [0] = (0,0)
                // [1] = (1,0)
                // [2] = (0,1)
                // [3] = (1,1)

                // Calculate which quad to choose
                choosenChild = bounds.x() - m_position.x() < subSize ? 0 : 1;
                if (bounds.y() - m_position.y() >= subSize)
                    choosenChild += 2;
            }
        }

        return choosenChild;
    }

    //--------------------------
    u32 findChildIndex(const Area<s32, 3> & bounds)
    {
        u32 choosenChild = INVALID_CHILD;
        s32 subSize = m_size / 2;

        // Test if bounds would fit in one sub-quad:
        // If the size fits regardless of a position
        if (bounds.width() < subSize &&
            bounds.height() < subSize &&
            bounds.depth() < subSize)
        {
            Vector<s32,3> subCenter(
                m_position.x() + subSize,
                m_position.y() + subSize,
                m_position.z() + subSize
            );

            // If the position doesn't makes bounds to cross an edge
            if (bounds.contains(subCenter))
            {
                // Calculate which quad to choose
                choosenChild = bounds.x() - m_position.x() < subSize ? 0 : 1;
                if (bounds.y() - m_position.y() >= subSize)
                    choosenChild += 2;
                if (bounds.z() - m_position.z() >= subSize)
                    choosenChild += 4;
            }
        }

        return choosenChild;
    }

    //--------------------------
    NTreeNode * findNode(const Area<s32, D> & bounds)
    {
        NTreeNode * node = this;
        u32 childIndex = node->findQuadIndex(bounds);

        while (childIndex != INVALID_QUAD)
        {
            node = node->m_children[quadIndex];
            childIndex = node->findChildIndex(bounds);
        }

        return node;
    }

private:
    SpaceTreeSettings & r_settings;
    Vector<s32, D> m_position;

    // Lookup example when D=3:
    //  i     x y z
    // [0] = (0,0,0)
    // [1] = (1,0,0)
    // [2] = (0,1,0)
    // [3] = (1,1,0)
    // [4] = (0,0,1)
    // [5] = (1,0,1)
    // [6] = (0,1,1)
    // [7] = (1,1,1)
    // There can't be null and other non-null children at a time.
    NTreeNode * m_children[CHILD_COUNT];
};

} // namespace sn

#endif // __HEADER_SN_SPACE_NTREENODE__

