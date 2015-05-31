#include "Profiler.h"
#include "Log.hpp"
#include "../util/assert.hpp"
#include <fstream>

namespace sn
{

//------------------------------------------------------------------------------
Profiler & Profiler::get()
{
	static Profiler s_instance;
	return s_instance;
}

//------------------------------------------------------------------------------
void Profiler::setEnabled(bool e)
{
	m_enabled = e;
}

//------------------------------------------------------------------------------
void Profiler::beginSample(const char * name, const char * file, s32 line, const char * customName)
{
	if (!m_enabled)
		return;

	Clock overheadClock;
	u32 parentIndex = getParentSample();

	u32 i = m_samples.size();

	m_samples.push_back(Sample());
	Sample & s = m_samples.back();
	
	s.name = name;
	s.file = file;
	s.line = line;
	s.customName = customName;
	s.beginTime = m_clock.getElapsedTime();
	s.depth = m_sampleStack.size();

	m_sampleStack.push(i);

	// Add the overhead on the parent sample
	if (parentIndex != -1)
		m_samples[parentIndex].overheadTime += overheadClock.getElapsedTime();
}

//------------------------------------------------------------------------------
void Profiler::endSample()
{
	if (!m_enabled)
		return;

	if(!m_sampleStack.empty())
	{
		u32 i = m_sampleStack.top();
		m_sampleStack.pop();
		Sample & s = m_samples[i];

		s.endTime = m_clock.getElapsedTime();
	}
}

//------------------------------------------------------------------------------
void Profiler::markFrame()
{
    if (m_enabled)
        m_frameMarks.push_back(m_samples.empty() ? 0 : m_samples.size() - 1);
}

//------------------------------------------------------------------------------
u32 Profiler::getParentSample() const
{
	if (m_sampleStack.empty())
		return -1;
	return m_sampleStack.top();
}

//------------------------------------------------------------------------------
void Profiler::dump(const char * filename, DumpMode mode) const
{
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	if (ofs.good())
	{
		dump(ofs, mode);
	}
	else
	{
		SN_ERROR("Couldn't dump profiling data to file '" << filename << '\'');
	}
}

//------------------------------------------------------------------------------
void Profiler::dump(std::ostream & os, DumpMode mode) const
{
	SN_LOG("Dumping profiling data...");

	switch (mode)
	{
	case DUMP_JSON:
		dumpJson(os);
		break;

	default:
		SN_ERROR("Unknown profiler dump mode");
		break;
	}
}

//------------------------------------------------------------------------------
//void serialize(JsonBox::Value & o, const Profiler::Sample & sample)
//{
//	o["file"].setString(sample.file);
//	o["line"].setInt(sample.line);
//	o["depth"].setInt(sample.depth);
//	o["beginTime"].setInt(static_cast<s32>(sample.beginTime.asMicroseconds())); // JSON love
//	o["endTime"].setInt(static_cast<s32>(sample.endTime.asMicroseconds()));
//	o["overheadTime"].setInt(static_cast<s32>(sample.overheadTime.asMicroseconds()));
//
//	if (sample.name)
//		o["name"].setString(sample.name);
//
//	if (sample.customName)
//		o["customName"].setString(sample.customName);
//}

//------------------------------------------------------------------------------
//void Profiler::dumpJson(std::ostream & os) const
//{
//    JsonBox::Value jsonSamples;
//	size_t i = 0;
//	for (auto it = m_samples.begin(); it != m_samples.end(); ++it)
//	{
//		Sample sample = *it;
//		serialize(jsonSamples[i++], sample);
//	}
//
//    JsonBox::Value jsonFrames;
//    size_t frameIndex = 0;
//    for (auto it = m_frameMarks.begin(); it != m_frameMarks.end(); ++it)
//    {
//        jsonFrames[frameIndex].setInt(m_frameMarks[frameIndex]);
//        ++frameIndex;
//    }
//
//	JsonBox::Value jsonRoot;
//	jsonRoot["samples"] = jsonSamples;
//    jsonRoot["frames"] = jsonFrames;
//	jsonRoot.writeToStream(os, false);
//}

//------------------------------------------------------------------------------
void serializeJson(std::ostream & os, const char * str)
{
    u32 i = 0;
    while (str[i] != '\0')
    {
        char c = str[i++];
        if (c == '\\')
            os << "\\\\";
        else if (c == '\n')
            os << "\\n";
        else
            os << c;
    }
}

//------------------------------------------------------------------------------
void serializeJson(std::ostream & os, const Profiler::Sample & s)
{
    os << "{\"name\":";
    serializeJson(os, s.name);

    os << ",\"customName\":";
    serializeJson(os, s.customName);

    os << ",\"file\":";
    serializeJson(os, s.file);

    os << ",\"line\":" << s.line
        << ",\"depth\":" << s.depth
        << ",\"beginTime\":" << s.beginTime.asMicroseconds()
        << ",\"endTime\":" << s.endTime.asMicroseconds()
        << ",\"overheadTime\":" << s.overheadTime.asMicroseconds()
        << "}";
}

//------------------------------------------------------------------------------
void Profiler::dumpJson(std::ostream & os) const
{
    os << "{\"frames\":[";
    
    if (!m_frameMarks.empty())
    {
        os << m_frameMarks[0];
        for (u32 i = 1; i < m_frameMarks.size(); ++i)
        {
            os << ',' << m_frameMarks[i];
        }
    }

    os << "],\"samples\":[";

    if (!m_samples.empty())
    {
        serializeJson(os, m_samples[0]);
        for (u32 i = 1; i < m_samples.size(); ++i)
        {
            os << ',';
            serializeJson(os, m_samples[i]);
        }
    }

    os << "]}";
}

//------------------------------------------------------------------------------
void Profiler::clear()
{
    m_samples.clear();
    while (!m_sampleStack.empty())
        endSample();
}

//------------------------------------------------------------------------------
const Profiler::Sample & Profiler::getLastSampleByDepth(u32 depth, u32 * out_index) const
{
    SN_ASSERT(!m_samples.empty(), "No samples recorded");
    for (u32 i = m_samples.size() - 1; i < m_samples.size(); --i)
    {
        const Sample & s = m_samples[i];
        if (s.depth == depth)
        {
            if (out_index)
                *out_index = i;
            return s;
        }
    }
    if (out_index)
        *out_index = 0;
    return m_samples[0];
}

} // namespace sn


