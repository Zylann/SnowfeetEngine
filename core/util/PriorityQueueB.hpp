/*
PriorityQueueB.hpp
Contribution by Marc GILLERON (2013)
Based on the work of Gustavo FRANCO (2006)
*/

#ifndef __HEADER_SN_PRIORITYQUEUEB__
#define __HEADER_SN_PRIORITYQUEUEB__

#include <vector>
#include <functional>
#include <core/types.hpp>

namespace sn
{

// Original implementation:
// http://www.codeguru.com/csharp/csharp/cs_misc/designtechniques/article.php/c12527/AStar-A-Implementation-in-C-Path-Finding-PathFinder.htm

// PriorityQueue with a non-template compare function.
// Warning: this was translated from C#. The comparer has been made public,
// but you may not assign it after construction (or data will become inconsistent).
// I leaved it this way in order to ease the use of lambdas, instead of a compile-time comparison.
template <typename T>
class PriorityQueueB
{
public:

    // Important: always assign this once before any pathfinding operation, never after
    std::function<s32(T,T)> m_comparer;

    PriorityQueueB()
    {
        m_comparer = [](T a, T b) { return a<b ? -1 : (a>b ? 1 : 0); };
    }

    PriorityQueueB(std::function<s32(T,T)> comparer)
    {
        m_comparer = comparer;
    }

    /// <summary>
    /// Push an object onto the PQ
    /// </summary>
    /// <param name="item">The new object</param>
    /// <returns>The index in the list where the object is _now_. This will change when objects are taken from or put onto the PQ.</returns>
    s32 push(T item)
    {
        int p = m_innerList.size(), p2;
        m_innerList.push_back(item); // E[p] = O
        do
        {
            if(p==0)
                break;
            p2 = (p-1)/2;
            if(onCompare(p,p2)<0)
            {
                switchElements(p,p2);
                p = p2;
            }
            else
                break;
        }
        while(true);
        return p;
    }

    /// <summary>
    /// Get the smallest object and remove it.
    /// </summary>
    /// <returns>The smallest object</returns>
    T pop()
    {
        T result = m_innerList[0];
        u32 p = 0, p1, p2, pn;
        m_innerList[0] = m_innerList.back();
        m_innerList.pop_back();

        do
        {
            pn = p;
            p1 = 2*p+1;
            p2 = 2*p+2;
            if(m_innerList.size()>p1 && onCompare(p,p1)>0) // links kleiner
                p = p1;
            if(m_innerList.size()>p2 && onCompare(p,p2)>0) // rechts noch kleiner
                p = p2;

            if(p==pn)
                break;
            switchElements(p,pn);
        }
        while(true);

        return result;
    }

    /// <summary>
    /// Notify the PQ that the object at position i has changed
    /// and the PQ needs to restore order.
    /// </summary>
    /// <param name="i">The index of the changed object.</param>
    void update(int i)
    {
        s32 p = i, pn;
        s32 p1, p2;

        do	// aufsteigen
        {
            if(p==0)
                break;

            p2 = (p-1)/2;

            if(onCompare(p,p2)<0)
            {
                switchElements(p,p2);
                p = p2;
            }
            else
                break;

        } while(true);

        if(p<i)
        {
            return;
        }

        do	   // absteigen
        {
            pn = p;
            p1 = 2*p+1;
            p2 = 2*p+2;

            if(m_innerList.Count>p1 && onCompare(p,p1)>0) // links kleiner
                p = p1;
            if(m_innerList.Count>p2 && onCompare(p,p2)>0) // rechts noch kleiner
                p = p2;

            if(p==pn)
                break;

            switchElements(p,pn);

        } while(true);
    }

    /// <summary>
    /// Get the smallest object without removing it.
    /// </summary>
    /// <returns>The smallest object</returns>
    T peek()
    {
        if(m_innerList.size() > 0)
            return m_innerList[0];
        return T();
    }

    void clear()
    {
        m_innerList.clear();
    }

    s32 count()
    {
        return m_innerList.size();
    }

    void removeLocation(T item)
    {
        s32 index = -1;
        for(s32 i=0; i<m_innerList.size(); ++i)
        {
            if(m_comparer(m_innerList[i], item) == 0)
                index = i;
        }

        if (index != -1)
            m_innerList.erase(m_innerList.begin() + index);
    }

    T get(s32 index)
    {
        return m_innerList[index];
    }

    void set(s32 index, T value)
    {
        m_innerList[index] = value;
        update(index);
    }

private:

    std::vector<T> m_innerList;

    void switchElements(u32 i, u32 j)
    {
        T h = m_innerList[i];
        m_innerList[i] = m_innerList[j];
        m_innerList[j] = h;
    }

    s32 onCompare(s32 i, s32 j)
    {
        return m_comparer(m_innerList[i], m_innerList[j]);
    }

};

} // namespace sn

#endif // __HEADER_SN_PRIORITYQUEUEB__


