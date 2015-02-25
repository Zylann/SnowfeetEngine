#ifndef __HEADER_SN_SPACE_NTREE__
#define __HEADER_SN_SPACE_NTREE__

#include <core/space/NTreeNode.hpp>
#include <core/math/Area.hpp>
#include <core/math/Vector.hpp>
#include <unordered_map>

namespace sn
{

template <unsigned int D>
class NTree
{
public:
    //--------------------------
    NTree()
    {
        // Apply default settings
        applySettings(m_settings);
    }

    //--------------------------
    ~NTree()
    {
        clear();
    }

    //--------------------------
    void add(ISpacePartitionObject * obj, const Area<f32, D> & bounds)
    {
        std::vector<NTreeNode<D>*> nodes;
        getOrCreateNodes(bounds, nodes);
        Area<s32,D> integerBounds = convertObjectBounds(bounds);
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            NTreeNode<D> & node = **it;
            node.add(obj, integerBounds);
        }
    }

    //--------------------------
    void remove(ISpacePartitionObject * obj, const Area<f32, D> & bounds)
    {
        std::vector<NTreeNode<D>*> nodes;
        getOrCreateNodes(bounds, nodes);
        IntRect integerBounds = convertObjectBounds(bounds);

        std::vector<NTreeNode<D>*> emptyNodes;

        // Remove the object from found nodes
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            NTreeNode<D> & node = **it;
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

    //--------------------------
    void query(const FloatAABB & bounds, std::vector<ISpacePartitionObject*> & results)
    {
        std::vector<NTreeNode<D>*> nodes;
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

    //--------------------------
    void clear()
    {
        for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
            delete it->second;
        m_roots.clear();
    }

    //--------------------------
    inline const SpaceTreeSettings & getSettings() const { return m_settings; }

    //--------------------------
#ifdef SN_BUILD_DEBUG    
    void debugPrint(std::ostream & os)
    {
        Area<s32,D> keyBounds = calculateTotalKeyBounds();
        os << "Roots: " << m_roots.size() << std::endl;
        os << "KeyBounds: " << keyBounds.toString() << std::endl;
    }
#endif

private:
    //--------------------------
    struct Boundaries
    {
        Vector<s32, D> min;
        Vector<s32, D> max;
    };

    //--------------------------
    Boundaries getConvertedBoundaries(const Area<f32, D> & bounds)
    {
        Boundaries b;
        f32 k = static_cast<f32>(m_settings.getRootSize() * m_settings.getWorldScale());
        for (u32 d = 0; d < D; ++d)
        {
            b.min[d] = static_cast<s32>(floor(bounds.origin()[d] / k));
            b.max[d] = static_cast<s32>(ceil((bounds.origin()[d] + bounds.size()[d]) / k));
        }
        return b;
    }

    //--------------------------
    void getNodes(const Area<f32, 2> & bounds, std::vector<NTreeNode<D>*> & outNodes)
    {
		SN_STATIC_ASSERT(D == 2);
        Boundaries b = getConvertedBoundaries(bounds);
        for (s32 y = b.min.y(); y < b.max.y(); ++y)
        {
            for (s32 x = b.min.x(); x < b.max.x(); ++x)
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

    //--------------------------
    void getNodes(const Area<f32, 3> & bounds, std::vector<NTreeNode<D>*> & outNodes)
    {
		SN_STATIC_ASSERT(D == 3);
        Boundaries b = getConvertedBoundaries(bounds);
        for (s32 z = b.min.z(); z < b.max.z(); ++z)
        {
            for (s32 y = b.min.y(); y < b.max.y(); ++y)
            {
                for (s32 x = b.min.x(); x < b.max.x(); ++x)
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

    //--------------------------
    void getOrCreateNodes(const Area<f32, 2> & bounds, std::vector<NTreeNode<D>*> & outNodes)
    {
		SN_STATIC_ASSERT(D == 2);
        Boundaries b = getConvertedBoundaries(bounds);
        for (s32 y = b.min.y(); y < b.max.y(); ++y)
        {
            for (s32 x = b.min.x(); x < b.max.x(); ++x)
            {
                Vector2i pos(x, y);
                auto it = m_roots.find(pos);
                if (it == m_roots.end())
                {
                    NTreeNode<D> * node = new NTreeNode<D>(m_settings, pos, m_settings.getRootSize(), 0);
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

    //--------------------------
    void getOrCreateNodes(const Area<f32, 3> & bounds, std::vector<NTreeNode<D>*> & outNodes)
    {
		SN_STATIC_ASSERT(D == 3);
        Boundaries b = getConvertedBoundaries(bounds);
        for (s32 z = b.min.z(); z < b.max.z(); ++z)
        {
            for (s32 y = b.min.y(); y < b.max.y(); ++y)
            {
                for (s32 x = b.min.x(); x < b.max.x(); ++x)
                {
                    Vector3i pos(x, y, z);
                    auto it = m_roots.find(pos);
                    if (it == m_roots.end())
                    {
                        NTreeNode<D> * node = new NTreeNode<D>(m_settings, pos, m_settings.getRootSize(), 0);
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

    //--------------------------
    Area<s32, D> calculateTotalKeyBounds()
    {
        Area<s32,D> bounds;
        for (auto it = m_roots.begin(); it != m_roots.end(); ++it)
        {
            NTreeNode & node = *(it->second);
            Vector<s32,D> pos = it->first;
            bounds.addPoint(pos);
        }
        return bounds;
    }

    //--------------------------
    void applySettings(const SpaceTreeSettings & newSettings)
    {
        m_settings = newSettings;
        m_settings.fix();
    }

    //--------------------------
    Area<s32, D> convertObjectBounds(const Area<f32, D> & bounds)
    {
        Area<s32,D> convertedBounds;
        for (u32 d = 0; d < D; ++d)
        {
            convertedBounds.origin()[d] = static_cast<s32>(bounds.origin()[d] * m_settings.getWorldScale());
            convertedBounds.size()[d] = static_cast<s32>(bounds.size()[d] * m_settings.getWorldScale());
        }
        return convertedBounds;
    }

private:
    SpaceTreeSettings m_settings;
    std::unordered_map<Vector<s32, D>, NTreeNode<D>*> m_roots;

};

} // namespace sn

#endif // __HEADER_SN_SPACE_NTREE__

