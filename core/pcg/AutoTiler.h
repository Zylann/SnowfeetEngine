/*
AutoTiler.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_AUTOTILER__
#define __HEADER_SN_AUTOTILER__

#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include <core/util/Array2D.h>

#include <JsonBox.h>

namespace sn
{

// TODO Move this to a ProceduralGen2D module

/// \brief Data-oriented auto-tiler.
/// Given an input grid, finds which tiles apply to them as an output grid.
/// (can handle connections, variants and default tiles)
/// The approach here is to define all possible cases instead of using
/// fixed IF block or bitwise presets. It consumes more memory, but that's
/// an acceptable tradeoff for handling almost all possible cases and being very flexible.
class SN_API AutoTiler
{
public:

    typedef u8 In_T;
    typedef u32 Out_T;

    // A connection mask represents a 3x3 grid (without the center) where
    // 1 are connecting tiles, 0 are other tiles.
    // 0 1 2
    // 3 x 4
    // 5 6 7
    typedef u8 ConnectionMask; // 01234567 (8 bits)

    // Defines the rules applied to a type of input cell depending on various neighborings,
    // in order to choose the good output value going at the center, from a series of possible variants
    struct SN_API RuleSet
    {
        // Tiles used if none of the rules match
        std::vector<Out_T> defaultOutput;

        // Relation between a specific connection neighboring and the tiles that can be used
        std::unordered_map<ConnectionMask, std::vector<Out_T>> cases; // [neighboring][variant]

        // Input values that should connect
        std::unordered_set<In_T> connections;

        // TODO add neighboring pattern option

        RuleSet(Out_T pDefaultOutput = 0):
            defaultOutput(pDefaultOutput)
        {
            defaultOutput.push_back(0);
        }

        void setDefaultOutput(Out_T singleValue)
        {
            if(defaultOutput.size() != 1)
            {
                defaultOutput.resize(1);
            }
            defaultOutput[0] = singleValue;
        }

        // Adds a neighboring case having certain neighbors set as "don't care".
        // This is done by specifying a bitmask where 1 means "take into account"
        // and 0 maens "don't care". If at least one bit is 0, all possible
        // fully-defined cases will be generated. In the end, it's like
        // calling the addCase(neighboring,variants) function a lot of times.
        void addCase(ConnectionMask neighboring, u8 mask, std::vector<Out_T> variants);

        // Adds a new neighboring case to the set with its resulting output variants.
        // Every value are relevant in the neighboring.
        // Returns true if added, false if existing (and then not added)
        bool addCase(ConnectionMask neighboring, std::vector<Out_T> variants)
        {
            auto it = cases.find(neighboring);
            if(it == cases.end())
            {
                cases.insert(std::make_pair(neighboring, variants));
                return true;
            }
            return false;
        }
    };

    In_T defaultInput; // Type used when out of bounds
    Out_T defaultOutput; // Tile used when no rules match

    // Tiling rules for each type in input grids
    std::vector<RuleSet> ruleSets;

    AutoTiler() :
        defaultInput(0),
        defaultOutput(0)
    {}

    void addRuleSet(In_T inputValue, const RuleSet & ruleset)
    {
        if(inputValue >= ruleSets.size())
        {
            ruleSets.resize(inputValue+1);
        }
        ruleSets[inputValue] = ruleset;
    }

    // Converts a grid of types into a grid of tiles
    void process(const Array2D<In_T> & inputGrid, Array2D<Out_T> & outputGrid) const;

    // Calculates a tile from its type at the given position
    Out_T processTile(const Array2D<In_T> & inputGrid, u32 x, u32 y) const;

    // Loads pattern data from a JSON file
    void unserialize(JsonBox::Value & o);

    static void stringToCaseKey(const std::string & s, ConnectionMask & conMask, u8 & dontCareMask);

};

} // namespace sn


#endif // __HEADER_SN_AUTOTILER__




