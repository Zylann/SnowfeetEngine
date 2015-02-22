#ifndef __HEADER_SN_SPACE_QUADTREESETTINGS__
#define __HEADER_SN_SPACE_QUADTREESETTINGS__

#include <core/types.hpp>

namespace sn
{

class SN_API QuadTreeSettings
{
public:
    QuadTreeSettings() :
        m_maxDepth(4),
        m_rootSize(1024),
        m_worldScale(1)
    {}

    void setMaxDepth(u32 maxDepth);
    void setRootSize(s32 rootSize);

    inline u32 getMaxDepth() const { return m_maxDepth; }
    inline s32 getRootSize() const { return m_rootSize; }

    inline f32 getWorldScale() const { return m_worldScale; }

    /// \brief Adjusts settings values for better performance
    void fix();

private:
    /// \brief Maximum depth of the tree.
    u32 m_maxDepth;
    /// \brief Size of the biggest quad in the tree. Must be power of two.
    s32 m_rootSize;
    /// \brief Scale factor before AABBs become converted to integer representation.
    f32 m_worldScale;

};

} // namespace sn

#endif // __HEADER_SN_SPACE_QUADTREESETTINGS__

