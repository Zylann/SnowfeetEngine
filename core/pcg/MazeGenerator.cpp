#include <list>
#include <core/pcg/MazeGenerator.hpp>
#include <core/math/Vector2.hpp>
#include <core/util/Log.hpp>

namespace sn
{

MazeGenerator::MazeGenerator(u32 width, u32 height)
{
    grid.create(width, height, 0);
}

//------------------------------------------------------------------------------
void MazeGenerator::generate(u32 seedX, u32 seedY)
{
    u32 iterations = 0;
    u32 maxIterations = 100000; // Infinite loop security

    // By default, the grid is full of blocking cells (0).

    Vector2i pos(seedX, seedY);
    u32 dir = -1;
    std::vector<Vector2i> startNodes;
    startNodes.push_back(Vector2i(seedX,seedY));

    u32 area = grid.area();
    for(u32 i = 0; i < area; ++i)
    {
        grid[i] &= ~Direction::ANY_BITS; // Clear directions
        grid[i] |= UNVISITED_BIT; // Set unvisited bit
    }

    // While there is registered positions where we can start a new cooridor (start nodes)
    while(!startNodes.empty())
    {
        // Choose at random from the available start nodes
        u32 j = math::rand(0, startNodes.size()-1);
        pos = startNodes[j];
        startNodes.erase(startNodes.begin() + j);

        // Randomize the maximum length of the corridor
        u32 corridorLength = math::rand(corridorLengthMin, corridorLengthMax);

        // Generate the corridor by iteration
        for(u32 i = 0; i < corridorLength; ++i)
        {
            u32 l = grid.getLocation(pos.x(), pos.y());

            std::vector<u32> availableDirs = unvisitedDirections(pos.x(), pos.y());

            // If there is at least one available direction
            if(!availableDirs.empty())
            {
                // If there is more than one direction available
                // Note: future going back (loop) will never happen because the cell we are on will
                // be be marked as visited, and so will not occur in the list.
                if(availableDirs.size() > 1)
                {
                    // Memorize the position to start from later
                    startNodes.push_back(pos);
                }

                // Choose a random direction
                dir = availableDirs[math::rand(0, availableDirs.size()-1)];

                // Add the available direction to cell's mask (from)
                if((grid[l] & UNVISITED_BIT) != 0)
                    grid[l] = 0;
                grid[l] |= (1 << dir);

                // Advance
                pos += Direction::toVector<s32>(dir);

                // Add the available direction to cell's mask (to)
                l = grid.getLocation(pos.x(), pos.y());
                if((grid[l] & UNVISITED_BIT) != 0)
                    grid[l] = 0;
                grid[l] |= (1 << Direction::opposite(dir));

                if(i+1 == corridorLength)
                {
                    // If it's the last iteration, add the current position in start nodes.
                    // If there are no direction available, the position will be discarded in further iterations.
                    startNodes.push_back(pos);
                }
            }
            else
            {
                // No directions available, finish corridor
                break;
            }
        }

        ++iterations;
        if(iterations >= maxIterations)
        {
            SN_ERROR("MazeGenerator: Reached max iterations");
            break;
        }
    }

    if(loopChance > 0.0001f)
    {
        connectRandomNodes(loopChance);
    }
}

//------------------------------------------------------------------------------
std::vector<u32> MazeGenerator::unvisitedDirections(s32 x, s32 y)
{
    std::vector<u32> dirs;

    for(u32 d = 0; d < 4; ++d)
    {
        Vector2i vec = Direction::toVector<s32>(d);
        s32 nx = x + vec.x();
        s32 ny = y + vec.y();

        if(grid.contains(nx,ny))
        {
            u32 c = grid.getNoEx(nx, ny);
            // Is the cell unvisited and enabled?
            if((c & UNVISITED_BIT)!=0 && (c & DISABLED_BIT)==0)
            {
                dirs.push_back(d);
            }
        }
    }

    return dirs;
}

//------------------------------------------------------------------------------
void MazeGenerator::connectRandomNodes(f32 chance)
{
    bool gen = false;
    for(s32 y  = 0; y < static_cast<s32>(grid.sizeY()); ++y)
    {
        for(s32 x  = 0; x < static_cast<s32>(grid.sizeX()); ++x)
        {
            u32 c = grid.getNoEx(x,y);

            // If the cell is a dead-end
            if(c == Direction::RIGHT_BIT ||
                c == Direction::LEFT_BIT ||
                c == Direction::DOWN_BIT ||
                c == Direction::UP_BIT)
            {
                // Connecting can fail, so we try to do it here and at the next iteration until it works.
                // In that case, gen is set to false.

                gen = gen | (math::randf(0.f, 1.f) < chance);

                if(gen)
                {
                    std::vector<u32> unavailableDirs;

                    if(c & Direction::RIGHT_BIT)
                    {
                        unavailableDirs.push_back(Direction::LEFT);
                    }
                    if(c & Direction::LEFT_BIT)
                    {
                        unavailableDirs.push_back(Direction::RIGHT);
                    }
                    if(c & Direction::DOWN_BIT)
                    {
                        unavailableDirs.push_back(Direction::DOWN);
                    }
                    if(c & Direction::UP_BIT)
                    {
                        unavailableDirs.push_back(Direction::UP_BIT);
                    }

                    if(!unavailableDirs.empty())
                    {
                        u32 dir = unavailableDirs[math::rand(0, unavailableDirs.size()-1)];

                        s32 nx = x + Direction::toVector<s32>(dir).x();
                        s32 ny = y + Direction::toVector<s32>(dir).y();

                        if(grid.contains(nx,ny))
                        {
                            grid[grid.getLocation(x, y)] |= (1 << dir);
                            grid[grid.getLocation(nx, ny)] |= (1 << Direction::opposite(dir));
                            gen = false;
                        }
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
#ifdef SN_SFML

void MazeGenerator::bakeTexture(sf::Image & tex)
{
    tex.create(3*grid.sizeX(), 3*grid.sizeY(), sf::Color(0,0,0,0));

    // Draw paths
    for(u32 y  = 0; y < grid.sizeY(); ++y)
    {
        for(u32 x  = 0; x < grid.sizeX(); ++x)
        {
            u32 px = x*3;
            u32 py = y*3;

            u32 c = grid.getNoEx(x,y);

            // Background
            sf::Color color;
            if(c & DISABLED_BIT) // disabled?
            {
                color = sf::Color(128, 0, 0);
            }
            else if((c & UNVISITED_BIT)==0) // visited?
            {
                color = sf::Color::Black;
            }
            else
            {
                color = sf::Color(0, 0, 0, 128);
            }
            for(u32 x2 = px; x2 < px+3; ++x2)
            {
                for(u32 y2 = py; y2 < py+3; ++y2)
                {
                    tex.setPixel(x2, y2, color);
                }
            }

            // Path
            if(c & Direction::ANY_BITS)
            {
                tex.setPixel(px+1, py+1, sf::Color::White);

                for(u32 d = 0; d < 4; ++d)
                {
                    if(c & (1 << d))
                    {
                        Vector2i pos = Vector2i(px+1, py+1) + Direction::toVector<s32>(d);
                        tex.setPixel(pos.x(), pos.y(), sf::Color::White);
                    }
                }
            }
        }
    }
}

#endif


} // namespace sn


