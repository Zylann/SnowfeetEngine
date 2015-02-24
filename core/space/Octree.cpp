#include "Octree.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Octree::Octree()
{
    // Apply default settings
    applySettings(m_settings);
}

//------------------------------------------------------------------------------
Octree::~Octree()
{
    clear();
}

//------------------------------------------------------------------------------
void Octree::applySettings(const SpaceTreeSettings & newSettings)
{
    m_settings = newSettings;
    m_settings.fix();
}

//------------------------------------------------------------------------------
void Octree::add(ISpacePartitionObject * obj, const FloatAABB & bounds)
{
    std::vector<OctreeNode*> nodes;
    getOrCreateNodes(bounds, nodes);
    IntAABB integerBounds = convertObjectBounds(bounds);
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        OctreeNode & node = **it;
        node.add(obj, integerBounds);
    }
}

//------------------------------------------------------------------------------
IntAABB Octree::convertObjectBounds(const FloatAABB & bounds)
{
    return IntAABB::fromPositionSize(
        static_cast<s32>(bounds.x() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.y() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.z() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.width() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.height() * m_settings.getWorldScale()),
        static_cast<s32>(bounds.depth() * m_settings.getWorldScale())
    );
}

//------------------------------------------------------------------------------
void Octree::remove(ISpacePartitionObject * obj, const FloatAABB & bounds)
{
    std::vector<OctreeNode*> nodes;
    getOrCreateNodes(bounds, nodes);
    IntAABB integerBounds = convertObjectBounds(bounds);

    std::vector<OctreeNode*> emptyNodes;

    // Remove the object from found nodes
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        OctreeNode & node = **it;
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
void Octree::query(const FloatAABB & bounds, std::vector<ISpacePartitionObject*> & results)
{
    std::vector<OctreeNode*> nodes;
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
void Octree::clear()
{
    for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
        delete it->second;
    m_roots.clear();
}

//------------------------------------------------------------------------------
#ifdef SN_BUILD_DEBUG
void Octree::debugPrint(std::ostream & os)
{
    IntAABB keyBounds = calculateTotalKeyBounds();
    os << "Roots: " << m_roots.size() << std::endl;
    os << "KeyBounds: " << keyBounds.toString() << std::endl;
}
#endif // SN_BUILD_DEBUG

//------------------------------------------------------------------------------
Octree::Boundaries Octree::getConvertedBoundaries(const FloatAABB & bounds)
{
    return{
        floor(bounds.minX() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        floor(bounds.minY() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        floor(bounds.minZ() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        ceil(bounds.maxX() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        ceil(bounds.maxY() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale())),
        ceil(bounds.maxZ() / static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale()))
    };
}

//------------------------------------------------------------------------------
void Octree::getNodes(const FloatAABB & bounds, std::vector<OctreeNode*> & outNodes)
{
    Boundaries b = getConvertedBoundaries(bounds);
    for (s32 z = b.minZ; z < b.maxZ; ++z)
    {
        for (s32 y = b.minY; y < b.maxY; ++y)
        {
            for (s32 x = b.minX; x < b.maxX; ++x)
            {
                Vector3i pos(x, y, z);
                auto it = m_roots.find(pos);
                if (it != m_roots.end())
                {
                    outNodes.push_back(it->second);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void Octree::getOrCreateNodes(const FloatAABB & bounds, std::vector<OctreeNode*> & outNodes)
{
    Boundaries b = getConvertedBoundaries(bounds);
    for (s32 z = b.minZ; z < b.maxZ; ++z)
    {
        for (s32 y = b.minY; y < b.maxY; ++y)
        {
            for (s32 x = b.minX; x < b.maxX; ++x)
            {
                Vector3i pos(x, y, z);
                auto it = m_roots.find(pos);
                if (it == m_roots.end())
                {
                    OctreeNode * node = new OctreeNode(*this, pos, m_settings.getRootSize(), 0);
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
}

//------------------------------------------------------------------------------
IntAABB Octree::calculateTotalKeyBounds()
{
    IntAABB bounds;
    for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
    {
        OctreeNode & node = *(it->second);
        Vector3i pos = it->first;
        bounds.addPoint(pos.x(), pos.y());
    }
    return bounds;
}

} // namespace sn

