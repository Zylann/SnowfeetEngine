/*
PathFinder.h
Contribution by Marc GILLERON (2013)
Based on the work of Gustavo FRANCO (2006)
*/

#ifndef __HEADER_SN_PATHFINDER__
#define __HEADER_SN_PATHFINDER__

#include <cassert>
#include <iostream>
#include <cmath>

#include <core/util/PriorityQueueB.h>

namespace sn
{

/// A* path finder optimized for grids.
/// Inspired on a similar implementation here :
/// http://www.codeguru.com/csharp/csharp/cs_misc/designtechniques/article.php/c12527/AStar-A-Implementation-in-C-Path-Finding-PathFinder.htm
/// Translated from C#.
class SN_API PathFinder
{
public:

    struct Node
    {
        s32 cost = 0; // goneCost + heuristic (F)
        s32 goneCost = 0; // (G)
        s32 heuristic = 0;
        s32 x = 0;
        s32 y = 0;
        s32 parentX = 0;
        s32 parentY = 0;
        s8 state = 0;
    };

    // Smaller version for internal use
    struct NodeFast
    {
        s32 cost = 0; // goneCost + heuristic (F)
        s32 goneCost = 0; // (G)
        s32 parentX = 0;
        s32 parentY = 0;
        s8 state = 0;
    };

public:

    PathFinder(s8 * grid, u16 gridSizeX, u16 gridSizeY)
    {
        assert(grid != nullptr);

        m_grid = grid;

        m_gridSizeX = gridSizeX;
        m_gridSizeY = gridSizeY;

        m_calcGrid = new NodeFast[m_gridSizeX * m_gridSizeY];

        // a and b are not actually compared :
        // I compare the grid values at [a] and [b].
        // A lambda is necessary to keep a reference to the grid and still have
        // a working encapsulated compare function.
        m_open.m_comparer = [=](s32 a, s32 b) {
            if(m_calcGrid[a].cost > m_calcGrid[b].cost)
                return 1;
            else if (m_calcGrid[a].cost < m_calcGrid[b].cost)
                return -1;
            return 0;
        };
    }

    ~PathFinder()
    {
        delete[] m_calcGrid;
    }

    inline void setSearchLimit(s32 lim) { m_searchLimit = lim; }
    inline void setDiagonals(bool enable) { m_diagonals = enable; }
    inline void setHeavyDiagonals(bool enable) { m_heavyDiagonals = enable; }
    inline void setAvoidDiagonalCross(bool enable) { m_avoidDiagonalCross = enable; }

private:

    inline s32 encodeLocation(s32 x, s32 y)
    {
        return y*m_gridSizeX+x;
    }

    inline void decodeLocation(s32 l, u16 & x, u16 & y)
    {
        x = (u16)(l % (s32)m_gridSizeX);
        y = (u16)(l / (s32)m_gridSizeX);
    }

public:

    std::vector<Node> * findPath(s32 startX, s32 startY, s32 endX, s32 endY);

private:

    s8 * m_grid = nullptr; // 0 means uncrossable, 1 means crossable, more means crossable at higher cost etc.
    u16  m_gridSizeX = 0;
    u16  m_gridSizeY = 0;
    s8   m_openNodeValue = 1;
    s8   m_closeNodeValue = 2;
    s32  m_searchLimit = 2000;
    bool m_diagonals = true;
    bool m_heavyDiagonals = true;
    bool m_avoidDiagonalCross = true;

    NodeFast *              m_calcGrid = nullptr;
    PriorityQueueB<s32>     m_open;
    std::vector<Node>       m_close;

    static const s8 s_directions[8][2];

};

} // namespace sn

#endif // __HEADER_SN_PATHFINDER__


