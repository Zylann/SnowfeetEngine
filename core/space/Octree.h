#ifndef __HEADER_SN_SPACE_OCTREE__
#define __HEADER_SN_SPACE_OCTREE__

#include <core/space/OctreeNode.h>
#include <core/space/SpaceTreeSettings.h>
#include <core/math/Vector2.h>
#include <unordered_map>

namespace sn
{

/// \brief QuadTree structure with unlimited bounds (my first implementation ever).
/// It implements a quad grid where each bucket can subdivide to a tree, depending on choosen settings.
class SN_API Octree : public ISpacePartitioner3D, public NonCopyable
{
public:
    Octree();
    ~Octree();

    void add(ISpacePartitionObject * obj, const FloatAABB & bounds) override;
    void remove(ISpacePartitionObject * obj, const FloatAABB & bounds) override;
    void query(const FloatAABB & bounds, std::vector<ISpacePartitionObject*> & results) override;
    void clear();

    inline const SpaceTreeSettings & getSettings() const { return m_settings; }

    // TODO setSettings(QuadTreeSettings)

#ifdef SN_BUILD_DEBUG    
    void debugPrint(std::ostream & os);
#endif

private:
    struct Boundaries
    {
        s32 minX;
        s32 minY;
        s32 minZ;
        s32 maxX;
        s32 maxY;
        s32 maxZ;
    };

    Boundaries getConvertedBoundaries(const FloatAABB & bounds);

    void getNodes(const FloatAABB & bounds, std::vector<OctreeNode*> & outNodes);
    void getOrCreateNodes(const FloatAABB & bounds, std::vector<OctreeNode*> & outNodes);

    IntAABB calculateTotalKeyBounds();

    void applySettings(const SpaceTreeSettings & newSettings);
    IntAABB convertObjectBounds(const FloatAABB & bounds);

private:
    SpaceTreeSettings m_settings;
    std::unordered_map<Vector3i, OctreeNode*> m_roots;

};

} // namespace sn

#endif // __HEADER_SN_SPACE_OCTREE__

