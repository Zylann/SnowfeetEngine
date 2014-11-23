#ifndef __HEADER_SN_HASHER2D__
#define __HEADER_SN_HASHER2D__

#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <core/math/Vector2.hpp>
#include <core/math/Rect.hpp>

namespace sn
{

/// \brief Container that maps two-dimensional elements in space,
/// making them faster to query within a specific area. Can be used for
/// collision detection or 2D culling for rendering.
///
/// This container internally divides the space in a regular grid of quads, called "buckets".
/// Each quad is identified by a pair of integer coordinates.
/// The bounds of objects are then mapped to this grid coordinate system
/// in order to calculate their spatial hash.
///
/// While QuadTrees are a common approach to the space partitionning problem,
/// a Hasher2D carries less overhead and is easier to code and maintain in many cases.
///
template <typename Element_T>
class SN_API Hasher2D
{
public:

    struct Bucket
    {
        std::unordered_set<Element_T> elements;
    };

    /// \brief Creates the hasher with a bucket size.
    /// \param bucketSize: size of a square division of space.
    /// For instance, an example size could be 2 or 3 times the size of the average element you'll store.
    Hasher2D(f32 bucketSize)
    {
        std::assert(bucketSize > 0);
        m_bucketSize = bucketSize;
    }

    /// \brief Registers an element in the hash space. No checks are made if it already is in.
    /// \param element: element to add.
    /// \param bounds: axis-aligned bounding box of the element.
    void insert(Element_T element, FloatRect bounds)
    {
        std::vector<Bucket*> buckets = getBuckets(bounds, true);
        for(auto it = buckets.begin(); it != buckets.end(); ++it)
        {
            it->elements.insert(element);
        }
    }

    /// \brief Removes an element in the hash space.
    /// \param element: element to remove.
    /// \param bounds: axis-aligned bounding box of the element.
    void remove(Element_T element, FloatRect bounds)
    {
        std::vector<Bucket*> buckets = getBuckets(bounds, false);
        for(auto it = buckets.begin(); it != buckets.end(); ++it)
        {
            it->elements.erase(element);
        }
    }

    /// \brief updates the bounds of an element that is already stored.
    /// This method should be called when the element moves or is resized.
    /// \param element: element to update
    /// \param oldBounds: bounds of the element before it changes
    /// \param newBounds: new bounds of the element
    void update(Element_T element, FloatRect oldBounds, FloatRect newBounds)
    {
        erase(element, oldBounds);
        insert(element, newBounds);
    }

    /// \brief gets all the elements within the specified bounds.
    /// Do not expect exact bounds, it's an approximation from a grid (see class description).
    /// Elements are de-duplicated if they are in more than one bucket.
    /// \param bounds: axis-aligned bounding box
    /// \return list of elements. Can be empty.
    std::unordered_set<Element_T> query(FloatRect bounds) const
    {
        std::unordered_set<Element_T> elements;
        std::vector<Bucket*> buckets = getBuckets(bounds, false);

        for(auto bucketIt = buckets.begin(); bucketIt != buckets.end(); ++bucketIt)
        {
            Bucket & bucket = *bucketIt;
            for(auto elemIt = bucket.elements.begin(); elemIt != bucket.elements.end(); ++elemIt)
            {
                elements.insert(*elemIt);
            }
        }

        return elements;
    }

    /// \brief removes all the elements that were registered in the Hasher2D.
    void clear()
    {
        m_buckets.clear();
    }

    /// \brief writes a text-representation of the Hasher2D for debug purpose.
    /// \param os: text stream where to write the info
    /// \param limit: max number of cells to display in the grid representation
    void printDebugState(std::ostream & os, s32 limit=80) const
    {
        IntRect bounds = getBucketTotalBounds();

        os << "---- Hasher2D ----" << std::endl;

        if(bounds.width() > limit || bounds.height() > limit)
        {
            log.warn() << "Hasher2D::printDebugState: "
                "limit reached (" << limit << "), "
                "cannot display the entire grid in text form." << log.endl();
        }

        Vector2i pos;
        for(pos.y() = bounds.minY(); pos.y() < bounds.maxY(); ++pos.y())
        {
            for(pos.x() = bounds.minX(); pos.x() < bounds.maxX(); ++pos.x())
            {
                auto it = m_buckets.find(pos);
                if(it != m_buckets.end())
                {
                    u32 n = it->second.size();
                    if(n < 10)
                        os << n;
                    else
                        os << "*";
                }
                else
                {
                    os << "-";
                }
                os << " ";
            }
            os << std::endl;
        }

        os << "bucket count: " << m_buckets.size() << std::endl;
        os << "bucket area: " << bounds << std::endl;

        os << "------------------" << std::endl;
    }

private:

    /// \brief Gets the total bounding rectangle in bucket space.
    IntRect getBucketTotalBounds() const
    {
        IntRect bounds;
        for(auto it = m_buckets.end(); it != m_buckets.end(); ++it)
        {
            Vector2i bucketPos = it->first;

            if(bounds.minX() > bucketPos.x())
                bounds.x = bucketPos.x();
            if(bounds.minY() > bucketPos.y())
                bounds.y = bucketPos.y();

            if(bounds.maxX() <= bucketPos.x())
                bounds.width = bucketPos.x() - bounds.minX() + 1;
            if(bounds.max.y <= bucketPos.y())
                bounds.height = bucketPos.y() - bounds.minY() + 1;
        }
        return bounds;
    }

    /// \brief Retrieves all the buckets intersecting the given bounds.
    /// \param bounds: axis-aligned bounding box
    /// \param createIfNotExist: create buckets if there is no mapped space?
    std::vector<Bucket*> getBuckets(FloatRect bounds, float createIfNotExist) const
    {
        std::vector<Bucket*> buckets;

        s32 minX = floor(bounds.minX() / m_bucketSize);
        s32 minY = floor(bounds.minY() / m_bucketSize);
        s32 maxX = ceil(bounds.maxX() / m_bucketSize);
        s32 maxY = ceil(bounds.maxY() / m_bucketSize);

        Vector2i pos;
        for(pos.y() = minY; pos.y() < maxY; ++pos.y())
        {
            for(pos.x() = minX; pos.x() < maxX; ++pos.x())
            {
                auto it = m_buckets.find(pos);
                if(it != m_buckets.end())
                {
                    buckets.push_back(&(it->second));
                }
                else if(createIfNotExist)
                {
                    it = m_buckets.insert(std::make_pair(pos, Bucket()));
                    buckets.push_back(&(it->second));
                }
            }
        }

        return buckets;
    }

    std::unordered_map<Vector2i> m_buckets;
    f32 m_bucketSize;

};

} // namespace sn

#endif // __HEADER_SN_HASHER2D__


