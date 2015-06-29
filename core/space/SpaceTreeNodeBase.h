#ifndef __HEADER_SN_SPACE_SPACETREENODEBASE__
#define __HEADER_SN_SPACE_SPACETREENODEBASE__

#include <core/util/NonCopyable.h>
#include <core/space/SpacePartitioner.h>
#include <unordered_set>

namespace sn
{

/// \brief Common code base for SpaceTreeNodes. Not meant to be used for polymorphism.
class SpaceTreeNodeBase : public NonCopyable
{
public:
    typedef std::unordered_set<ISpacePartitionObject*> ObjectsList;

    SpaceTreeNodeBase(u32 size, u32 depth) :
        m_size(size),
        m_depth(depth)
    {}

    inline const ObjectsList & getObjects() const { return m_objects; }

protected:
    s32 m_size;
    u32 m_depth;

    ObjectsList m_objects;
};

} // namespace sn

#endif // __HEADER_SN_SPACE_SPACETREENODEBASE__

