#ifndef __HEADER_SN_SPACE_SPACEPARTITIONER__
#define __HEADER_SN_SPACE_SPACEPARTITIONER__

#include <vector>
#include <core/math/Rect.hpp>

namespace sn
{

// TODO This is for 2D, generalize for 3D
class ISpacePartitioner
{
public:
    typedef void* Userdata;

    virtual ~ISpacePartitioner() {}

    virtual void add(Userdata obj, const FloatRect & bounds) = 0;
    virtual void remove(Userdata obj, const FloatRect & bounds) = 0;
    virtual void query(const FloatRect & bounds, std::vector<Userdata> & results) = 0;
    virtual void clear() = 0;

    virtual void move(Userdata obj, const FloatRect & oldBounds, const FloatRect & newBounds)
    {
        remove(obj, oldBounds);
        add(obj, newBounds);
    }

};

//class ISpacePartitionObject
//{
//public:
//    virtual ~ISpacePartitionObject() {}
//};

} // namespace sn

#endif // __HEADER_SN_SPACE_SPACEPARTITIONER__

