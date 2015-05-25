#include "Profiler.h"
#include "Log.hpp"
#include "../json/json_utils.hpp"
#include <fstream>

namespace sn
{

//------------------------------------------------------------------------------
void Profiler::beginSample(const char * name, const char * file, s32 line, const char * customName)
{
	if (!m_enabled)
		return;

	Clock overheadClock;
	u32 parentIndex = getParentSample();

	u32 i = 0;

	// Note: name should be guaranteed to be unique because it's a pointer to a string litteral
	
	Sample * samplePtr = nullptr;

	auto it = m_nameToSample.find(name);
	if(it == m_nameToSample.end())
	{
		i = m_samples.size();
		
		Sample s;
		s.name = name;
		s.file = file;
		s.line = line;
		s.hitCount = 1;
		s.customName = customName;

		m_samples.push_back(s);
		m_nameToSample[name] = i;

		samplePtr = &m_samples.back();
	}
	else
	{
		i = it->second;
		Sample & s = m_samples[i];

		++s.hitCount;

		samplePtr = &s;
	}

	m_sampleStack.push(i);

	SN_ASSERT(samplePtr != nullptr, "Invalid state");

	// Subtract the overhead on the parent sample
	if (parentIndex != -1)
		m_samples[parentIndex].totalTime -= overheadClock.getElapsedTime();

	samplePtr->clock.restart();
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

		s.totalTime += s.clock.getElapsedTime();
	}
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
void serialize(JsonBox::Value & o, const Profiler::Sample & sample)
{
	o["file"].setString(sample.file);
	o["line"].setInt(sample.line);
	o["hitCount"].setInt(sample.hitCount);
	o["totalTime"].setInt(static_cast<s32>(sample.totalTime.asMicroseconds()));
}

//------------------------------------------------------------------------------
void Profiler::dumpJson(std::ostream & os) const
{
	JsonBox::Value jsonSamples;

	size_t i = 0;
	for (auto it = m_samples.begin(); it != m_samples.end(); ++it)
	{
		Sample sample = *it;
		serialize(jsonSamples[i++], sample);
	}

	JsonBox::Value jsonRoot;
	jsonRoot["samples"] = jsonSamples;
	jsonRoot.writeToStream(os);
}

} // namespace sn


