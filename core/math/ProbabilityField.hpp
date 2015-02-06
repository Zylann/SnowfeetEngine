/*
ProbabilityField.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_PROBABILITY_FIELD__
#define __HEADER_SN_PROBABILITY_FIELD__

#include <core/util/Log.hpp>
#include <vector>

namespace sn
{

/// \brief Defines a set of numbered events (from 0 to N-1) having each a probability.
/// Given a random float number between 0 and 1, you can pick an event that will then
/// have the expected frequency.
/// For instance, if you defined event[0] = 1 and event[1] = 2,
/// event[1] will be picked up exactly twice more often than event[0].
class ProbabilityField
{
public:

    ProbabilityField(): m_needCompile(true)
    {}

    inline u32 size() const
    {
        return m_frequencies.size();
    }

    void setFrequency(u32 eventIndex, f32 chance)
    {
        if (eventIndex < m_frequencies.size())
        {
            m_frequencies.resize(eventIndex + 1, 0);
            m_cumul.resize(m_frequencies.size() + 1);
        }

        m_frequencies[eventIndex] = chance;
        m_needCompile = true;
    }

    f32 getFrequency(u32 eventIndex)
    {
        return m_frequencies[eventIndex];
    }

    u32 pick(float p)
    {
        if (m_needCompile)
        {
            compile();
            m_needCompile = false;
        }

        float t = p;

        for (int i = 1; i < m_cumul.size(); ++i)
        {
            if (t >= m_cumul[i - 1] && t < m_cumul[i])
                return i - 1;
        }

        return m_frequencies.size() - 1;
    }

private:

    void compile()
    {
        normalize();

        // Note: cumul[0] always == 0, and cumul[length-1] always == 1

        for (int i = 0; i < m_frequencies.size(); ++i)
        {
            m_cumul[i + 1] = m_cumul[i] + m_frequencies[i];
        }
    }

    void normalize()
    {
        float sum = 0;

        for (int i = 0; i < m_frequencies.size(); ++i)
        {
            sum += m_frequencies[i];
        }

        if (sum != 0)
        {
            for (int i = 0; i < m_frequencies.size(); ++i)
            {
                m_frequencies[i] /= sum;
            }
        }
        else
        {
            SN_ERROR("ProbabilityField: all values are set to zero!");
        }
    }

    bool m_needCompile;
    std::vector<f32> m_frequencies; // [N]
    std::vector<f32> m_cumul; // [N + 1];

};

} // namespace sn

#endif // __HEADER_SN_PROBABILITY_FIELD__

