/*
AutoTiler.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <core/pcg/AutoTiler.h>
#include <core/util/Log.h>
#include <core/json/json_serialize.h>

namespace sn
{

// 8-connexity neighboring vectors from center
const s32 g_nv8[8][2] = {
    {-1, -1},
    {0, -1},
    {1, -1},
    {-1, 0},
    {1, 0},
    {-1, 1},
    {0, 1},
    {1, 1}
};

//------------------------------------------------------------------------------
void AutoTiler::RuleSet::addCase(ConnectionMask neighboring, u8 mask, std::vector<Out_T> variants)
{
    // Is the case fully-specific?
    if(mask == 0xf)
    {
        // The mask is full, no need to generate cases
        addCase(neighboring, variants);
        return;
    }

    // Is the mask "don't care at all"?
    if(mask == 0x0)
    {
        // It means we don't even need to watch for neighboring,
        // just use the defaultOuput
        defaultOutput = variants;
        return;
    }

    // mask is neither full nor empty, then some neighbors are "don't care".
    // Generate all cases for these locations
    // (maximum combination count is 256, but it's the case handled by
    // defaultOutput already, so a real count would be 128, which is not so big).

    // Ensure "don't care" neighbors are unset (these will be generated)
    neighboring &= mask;

    // Count zero bits in the mask and memorize their position
    u8 bits[8]; // 1-bit masks representing the Nth variable bit
    u8 bitCount = 0;
    for(u8 i = 0; i < 8; ++i)
    {
        u8 bit = (1 << i);
        if(!(mask & bit))
        {
            bits[bitCount++] = bit;
        }
    }

    // TODO test case generation

    // Generate possible neighborings :
    // Use a number going from 0 to max, and dispatch its bits in the final
    // neighboring where bits are variable.
    //---------
    // Example
    //---------
    // Some case:           0b10100100
    //                          +
    // Mask:                0b11100101
    //                          =
    // Means:               0b101**1*0
    //
    // Needed combinations: 0b10100100  n = 0b000 (0)
    //                           ^^ ^
    //                      0b10100110  n = 0b001 (1)
    //                           ^^ ^
    //                      0b10101100  n = 0b010 (2)
    //                           ^^ ^
    //                      0b10101110  n = 0b011 (3)
    //                      ...
    //                      0b10111110  n = 0b111 (7)
    //                           ^^ ^
    u16 max = 1 << bitCount;
    for(u16 n = 0; n < max; ++n)
    {
        u8 generatedNeighboring = neighboring;
        for(u8 i = 0; i < bitCount; ++i)
        {
            u8 bit = (1 << i);
            if(n & bit)
            {
                generatedNeighboring |= bits[i];
            }
        }

        addCase(generatedNeighboring, variants);
    }
}

//------------------------------------------------------------------------------
void AutoTiler::process(const Array2D<In_T> & inputGrid, Array2D<Out_T> & outputGrid) const
{
    // For each cell in the grid
    for(s32 y = 0; y < static_cast<s32>(inputGrid.sizeY()); ++y)
    {
        for(s32 x = 0; x < static_cast<s32>(inputGrid.sizeX()); ++x)
        {
            outputGrid.set(x,y, processTile(inputGrid, x, y));
        }
    }
}

//------------------------------------------------------------------------------
AutoTiler::Out_T AutoTiler::processTile(const Array2D<In_T> & inputGrid, u32 x, u32 y) const
{
    // Get the type of the cell
    In_T type = inputGrid.getNoEx(x,y);

    Out_T outputValue = defaultOutput;

    // If the type is referenced
    if(type < ruleSets.size())
    {
        // If the type has no rules
        const RuleSet & rules = ruleSets[type];
        if(rules.cases.empty())
        {
            // Apply default outputValue to the cell
            outputValue = rules.defaultOutput[0];
        }
        else // The type has rules:
        {
            // Retrieve neighboring mask
            u8 m = 0;
            for(u32 i = 0; i < 8; ++i)
            {
                s32 nx = x + g_nv8[i][0];
                s32 ny = y + g_nv8[i][1];

                // Get neighboring value
                In_T ntype = inputGrid.contains(nx, ny) ? inputGrid.getNoEx(nx, ny) : defaultInput;

                // If it connects
                if(rules.connections.find(ntype) != rules.connections.end())
                {
                    // Add to the mask
                    m |= 1;
                }

                if(i != 7)
                {
                    // Shift the mask to enter next value on next loop
                    m <<= 1;
                }
            }

            ConnectionMask neighboring = m;

            // Find a rule for the given neighboring
            auto ruleIt = rules.cases.find(neighboring);
            if(ruleIt != rules.cases.end())
            {
                // Found a rule, apply a corresponding outputValue
                outputValue = ruleIt->second[0]; // TODO choose variant at random
            }
            else
            {
                // No rules for this case, use type's default outputValue
                outputValue = rules.defaultOutput[0];
            }
        }
    }

    return outputValue;
}

//------------------------------------------------------------------------------
void AutoTiler::stringToCaseKey(const std::string & s, ConnectionMask & conMask, u8 & dontCareMask)
{
    dontCareMask = 0xff;

    if(s.size() != 8)
    {
        SN_ERROR("AutoTiler::stringToCaseKey: invalid case string \"" << s << "\", expected size of 8");
        conMask = 0;
        return;
    }

    u8 bit = 1 << 7;
    for(u32 i = 0; i < s.size(); ++i)
    {
        char c = s[i];

        switch(c)
        {
        case '1':
            conMask |= bit;
            break;

        case '*':
            dontCareMask &= ~bit;
            break;

        default:
            break;
        }

        bit >>= 1;
    }
}

//------------------------------------------------------------------------------
void AutoTiler::unserialize(JsonBox::Value & o)
{
    defaultInput = o["defaultInput"].getInt();
    defaultOutput = o["defaultOutput"].getInt();

    JsonBox::Value & jRuleSets = o["ruleSets"];
    ruleSets.resize(jRuleSets.getArray().size());

    for(u32 i = 0; i < ruleSets.size(); ++i)
    {
        JsonBox::Value & jrs = jRuleSets[i];

        In_T input = i;
        if(!jrs["input"].isNull())
        {
            input = jrs["input"].getInt();
        }
        RuleSet & rs = ruleSets[input];

        // Default output
        sn::unserialize(jrs["defaultOutput"], rs.defaultOutput);

        // Connections
        sn::unserialize(jrs["connections"], rs.connections);

        // Cases
        JsonBox::Value & jcases = jrs["cases"];
        u32 jcasesSize = jcases.getArray().size();
        for(u32 j = 0; j < jcasesSize; ++j)
        {
            JsonBox::Value & jcase = jcases[j];

            u8 dontCareMask = 0;
            ConnectionMask conMask = 0;

            std::string s = jcase["n"].getString();
            stringToCaseKey(s, conMask, dontCareMask);

            std::vector<Out_T> variants;
            sn::unserialize(jcase["v"], variants);

            rs.addCase(conMask, dontCareMask, variants);
        }
    }
}

} // namespace sn




