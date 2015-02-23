#include "QuadTree.hpp"

namespace sn
{

//------------------------------------------------------------------------------
QuadTree::QuadTree()
{
    // Apply default settings
    applySettings(m_settings);
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree()
{
    clear();
}

//------------------------------------------------------------------------------
void QuadTree::applySettings(const SpaceTreeSettings & newSettings)
{
    m_settings = newSettings;
    m_settings.fix();
}

//------------------------------------------------------------------------------
void QuadTree::add(ISpacePartitionObject * obj, const FloatRect & bounds)
{
    std::vector<QuadTreeNode*> nodes;
    getOrCreateNodes(bounds, nodes);
    IntRect integerBounds = convertObjectBounds(bounds);
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        QuadTreeNode & node = **it;
        node.add(obj, integerBounds);
    }
}

//------------------------------------------------------------------------------
IntRect QuadTree::convertObjectBounds(const FloatRect & bounds)
{
    return IntRect::fromPositionSize(
        static_cast<s32>(bounds.x() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.y() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.width() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.height() * m_settings.getWorldScale())
    );
}

//------------------------------------------------------------------------------
void QuadTree::remove(ISpacePartitionObject * obj, const FloatRect & bounds)
{
    std::vector<QuadTreeNode*> nodes;
    getOrCreateNodes(bounds, nodes);
    IntRect integerBounds = convertObjectBounds(bounds);

    std::vector<QuadTreeNode*> emptyNodes;

    // Remove the object from found nodes
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        QuadTreeNode & node = **it;
        node.remove(obj, integerBounds);
        if (node.isEmpty())
            emptyNodes.push_back(&node);
    }

    // Remove empty roots
    if (!emptyNodes.empty())
    {
        for (auto it = emptyNodes.begin(); it != emptyNodes.end(); ++it)
            m_roots.erase((*it)->getPosition());
    }
}

//------------------------------------------------------------------------------
void QuadTree::query(const FloatRect & bounds, std::vector<ISpacePartitionObject*> & results)
{
    std::vector<QuadTreeNode*> nodes;
    getNodes(bounds, nodes);
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        const auto & nodeObjects = (*it)->getObjects();
        for (auto it2 = nodeObjects.begin(); it2 != nodeObjects.end(); ++it2)
        {
            results.push_back(*it2);
        }
    }
}

//------------------------------------------------------------------------------
void QuadTree::clear()
{
    for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
        delete it->second;
    m_roots.clear();
}

//------------------------------------------------------------------------------
#ifdef SN_BUILD_DEBUG
void QuadTree::debugPrint(std::ostream & os)
{
    IntRect keyBounds = calculateTotalKeyBounds();

    os << "Roots: " << m_roots.size() << std::endl;
    os << "KeyBounds: " << keyBounds.toString() << std::endl;

    s32 xlim = 79;
    s32 ylim = 79;

    if (keyBounds.width() > xlim)
        keyBounds.width() = xlim;
    if (keyBounds.height() > ylim)
        keyBounds.height() = ylim;

    os << "Grid: " << std::endl;
    for (s32 y = keyBounds.minY(); y < keyBounds.maxY(); ++y)
    {
        for (s32 x = keyBounds.minX(); x < keyBounds.maxX(); ++x)
        {
            auto it = m_roots.find(Vector2i(x, y));
            if (it == m_roots.end())
                os << "- ";
            else
                os << "O ";
        }
        os << std::endl;
    }
}
#endif // SN_BUILD_DEBUG

//------------------------------------------------------------------------------
QuadTree::Boundaries QuadTree::getConvertedBoundaries(const FloatRect & bounds)
{
    return {
        floor(bounds.minX() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        floor(bounds.minY() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        ceil(bounds.maxX() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        ceil(bounds.maxY() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale()))
    };
}

//------------------------------------------------------------------------------
void QuadTree::getNodes(const FloatRect & bounds, std::vector<QuadTreeNode*> & outNodes)
{
    Boundaries b = getConvertedBoundaries(bounds);
    for (s32 y = b.minY; y < b.maxY; ++y)
    {
        for (s32 x = b.minX; x < b.maxX; ++x)
        {
            Vector2i pos(x, y);
            auto it = m_roots.find(pos);
            if (it != m_roots.end())
            {
                outNodes.push_back(it->second);
            }
        }
    }
}

//------------------------------------------------------------------------------
void QuadTree::getOrCreateNodes(const FloatRect & bounds, std::vector<QuadTreeNode*> & outNodes)
{
    Boundaries b = getConvertedBoundaries(bounds);
    for (s32 y = b.minY; y < b.maxY; ++y)
    {
        for (s32 x = b.minX; x < b.maxX; ++x)
        {
            Vector2i pos(x, y);
            auto it = m_roots.find(pos);
            if (it == m_roots.end())
            {
                QuadTreeNode * node = new QuadTreeNode(*this, pos, m_settings.getRootSize(), 0);
                m_roots[pos] = node;
                outNodes.push_back(node);
            }
            else
            {
                outNodes.push_back(it->second);
            }
        }
    }
}

//------------------------------------------------------------------------------
IntRect QuadTree::calculateTotalKeyBounds()
{
    IntRect bounds;
    for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
    {
        QuadTreeNode & node = *(it->second);
        Vector2i pos = it->first;
        bounds.addPoint(pos.x(), pos.y());
    }
    return bounds;
}

} // namespace sn

