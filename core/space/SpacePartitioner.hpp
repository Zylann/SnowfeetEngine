#ifndef __HEADER_SN_SPACE_SPACEPARTITIONER__
#define __HEADER_SN_SPACE_SPACEPARTITIONER__

#include <vector>
#include <core/math/Rect.hpp>

namespace sn
{

//------------------------------------------------------------------------------
class SN_API ISpacePartitionObject
{
public:
	virtual ~ISpacePartitionObject() {}
};

//------------------------------------------------------------------------------
class SN_API ISpacePartitioner2D
{
public:
    virtual ~ISpacePartitioner2D() {}

    virtual void add(ISpacePartitionObject * obj, const FloatRect & bounds) = 0;
    virtual void remove(ISpacePartitionObject * obj, const FloatRect & bounds) = 0;
    virtual void query(const FloatRect & bounds, std::vector<ISpacePartitionObject*> & results) = 0;
    virtual void clear() = 0;

    virtual void move(ISpacePartitionObject * obj, const FloatRect & oldBounds, const FloatRect & newBounds)
    {
        remove(obj, oldBounds);
        add(obj, newBounds);
    }

};

//------------------------------------------------------------------------------
class SN_API ISpacePartitioner3D : public ISpacePartitioner2D
{
public:
    virtual ~ISpacePartitioner3D() {}

    virtual void add(ISpacePartitionObject * obj, const FloatAABB & bounds) = 0;
    virtual void remove(ISpacePartitionObject * obj, const FloatAABB & bounds) = 0;
    virtual void query(const FloatAABB & bounds, std::vector<ISpacePartitionObject*> & results) = 0;
    //virtual void clear() = 0;

	//----------------------------------------
	// Default implementations for 2D subset
	//----------------------------------------

    virtual void move(ISpacePartitionObject * obj, const FloatAABB & oldBounds, const FloatAABB & newBounds)
    {
        remove(obj, oldBounds);
        add(obj, newBounds);
    }

    virtual void add(ISpacePartitionObject * obj, const FloatRect & bounds)
    {
        add(obj, FloatAABB::fromPositionSize(
            bounds.x(), bounds.y(), 0, 
            bounds.width(), bounds.height(), 0)
        );
    }

    virtual void remove(ISpacePartitionObject * obj, const FloatRect & bounds)
    {
        remove(obj, FloatAABB::fromPositionSize(
            bounds.x(), bounds.y(), 0, 
            bounds.width(), bounds.height(), 0)
        );
    }

    virtual void query(const FloatRect & bounds, std::vector<ISpacePartitionObject*> & results)
    {
        query(
            FloatAABB::fromPositionSize(
                bounds.x(), bounds.y(), 0,
                bounds.width(), bounds.height(), 0
            ),
            results
        );
    }

};

} // namespace sn

#endif // __HEADER_SN_SPACE_SPACEPARTITIONER__

