/*
PathFinder.cpp
Contribution by Marc GILLERON (2013)
Based on the work of Gustavo FRANCO (2006)
*/


#include <core/util/PathFinder.h>
#include <core/util/Log.h>

namespace sn
{

const s8 PathFinder::s_directions[8][2] =
{
// Cardinals
    {0,-1},
    {1, 0},
    {0, 1},
    {-1,0},
// Diagonals
    {1,-1},
    {1,1},
    {-1,1},
    {-1,-1}
};

//------------------------------------------------------------------------------
std::vector<PathFinder::Node> * PathFinder::findPath(s32 startX, s32 startY, s32 endX, s32 endY)
{
    bool found = false;
    bool stop = false;
    s32 heuristicEstimate = 2;
    s32 closeNodeCounter = 0;
    s32 directionCount = m_diagonals ? 8 : 4;

    // Instead of clearing the grid each time, I change node state values and simply ignore the other values.
    // It's faster than clearing the grid (not much, but it is).
    m_openNodeValue += 2;
    m_closeNodeValue += 2;

    m_open.clear();
    m_close.clear();

    s32 location = encodeLocation(startX, startY);
    s32 endLocation = encodeLocation(endX, endY);
    u16 locationX=0, locationY=0;

    m_calcGrid[location].goneCost  = 0;
    m_calcGrid[location].cost      = 0 + heuristicEstimate;
    m_calcGrid[location].parentX   = (u16) startX;
    m_calcGrid[location].parentY   = (u16) startY;
    m_calcGrid[location].state     = m_openNodeValue;

    m_open.push(location);

    while(m_open.count() > 0 && !stop)
    {
        location = m_open.pop();

        // Is it in closed list? means this node was already processed
        if(m_calcGrid[location].state == m_closeNodeValue)
        {
            continue;
        }

        decodeLocation(location, locationX, locationY);

        if(location == endLocation)
        {
            m_calcGrid[location].state = m_closeNodeValue;
            found = true;
            break;
        }

        if(closeNodeCounter > m_searchLimit)
        {
            // Evaluated nodes exceeded limit : path not found
#ifdef SN_BUILD_DEBUG
            SN_DLOG("Pathfinder searchLimit exceed");
#endif
            return nullptr;
        }

        // Let's calculate each successors
        for(s32 i=0; i<directionCount; ++i)
        {
            u16 newLocationX = (u16) (locationX + s_directions[i][0]);
            u16 newLocationY = (u16) (locationY + s_directions[i][1]);
            s32 newLocation = encodeLocation(newLocationX, newLocationY);
            s32 newGoneCost;

            // Outside the grid?
            if(newLocationX >= m_gridSizeX || newLocationY >= m_gridSizeY)
                continue;

            // Not crossable?
            if(m_grid[newLocation] == 0)
                continue;

            if(m_avoidDiagonalCross)
            {
                //
                //   +----+----+----+
                //   |    |    | 3  |
                //   |    |    |    |
                //   +----+----+----+
                //   |    | 2  |XXXX|   Diagonals are allowed,
                //   |    |    |XXXX|   but going through 1, 2 then 3 should be avoided,
                //   +----+----+----+   because there are contiguous uncrossable cells.
                //   |    | 1  |XXXX|   (A square object cannot go from 2 to 3 for example, it will have to bypass the corner).
                //   |    |    |XXXX|
                //   +----+----+----+
                //

                if(i > 3)
                {
                    if( m_grid[encodeLocation(locationX+s_directions[i][0], locationY)] == 0 ||
                        m_grid[encodeLocation(locationX, locationY+s_directions[i][1])] == 0)
                    {
                        continue;
                    }
                }
            }

            if(m_heavyDiagonals && i>3)
            {
                newGoneCost = m_calcGrid[location].goneCost + (s32) (m_grid[newLocation] * 2.41f);
            }
            else
            {
                newGoneCost = m_calcGrid[location].goneCost + m_grid[newLocation];
            }

            //Is it open or closed?
            if(m_calcGrid[newLocation].state == m_openNodeValue || m_calcGrid[newLocation].state == m_closeNodeValue)
            {
                // The current node has less code than the previous? then skip this node
                if(m_calcGrid[newLocation].goneCost <= newGoneCost)
                {
                    continue;
                }
            }

            m_calcGrid[newLocation].parentX = locationX;
            m_calcGrid[newLocation].parentY = locationY;
            m_calcGrid[newLocation].goneCost = newGoneCost;

            // Heuristic : manhattan distance
            s32 heuristic = heuristicEstimate * (std::abs(newLocationX - endX) + std::abs(newLocationY - endY));

            m_calcGrid[newLocation].cost = newGoneCost + heuristic;

            //It is faster if we leave the open node in the priority queue
            //When it is removed, it will be already closed, it will be ignored automatically
            m_open.push(newLocation);

            m_calcGrid[newLocation].state = m_openNodeValue;
        }

        ++closeNodeCounter;
        m_calcGrid[location].state = m_closeNodeValue;
    }

    if(found)
    {
        m_close.clear();

        int posX = endX;
        int posY = endY;

        NodeFast tmpNode = m_calcGrid[encodeLocation(endX, endY)];

        Node node;
        node.cost = tmpNode.cost;
        node.goneCost = tmpNode.goneCost;
        node.heuristic  = 0;
        node.parentX = tmpNode.parentX;
        node.parentY = tmpNode.parentY;
        node.x = endX;
        node.y = endY;

        while(node.x != node.parentX || node.y != node.parentY)
        {
            m_close.push_back(node);

            posX = node.parentX;
            posY = node.parentY;

            tmpNode = m_calcGrid[encodeLocation(posX, posY)];
            node.cost = tmpNode.cost;
            node.goneCost = tmpNode.goneCost;
            node.heuristic = 0;
            node.parentX = tmpNode.parentX;
            node.parentY = tmpNode.parentY;
            node.x = posX;
            node.y = posY;
        }

        m_close.push_back(node);

        // Path found
        return &m_close;
    }

    // Path not found
#ifdef SN_BUILD_DEBUG
    SN_DLOG("Pathfinder path not found");
#endif

    return nullptr;
}

} // namespace sn


