#include "Profiler.h"
#include "Log.hpp"
#include "../json/json_utils.hpp"
#include <fstream>

namespace sn
{

//------------------------------------------------------------------------------
void Profiler::beginSample(const char * name, const char * file, s32 line)
{
	if (!m_enabled)
		return;

	u32 i = 0;

	// Note: name should be guaranteed to be unique because it's a pointer to a string litteral

	auto it = m_nameToSample.find(name);
	if(it == m_nameToSample.end())
	{
		i = m_samples.size();
		
		Sample s;
		s.name = name;
		s.file = file;
		s.line = line;
		s.hitCount = 1;

		m_samples.push_back(s);
		m_nameToSample[name] = i;
	}
	else
	{
		i = it->second;
		Sample & s = m_samples[i];

		++s.hitCount;
		s.clock.restart();
	}

	m_sampleStack.push(i);
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
void Profiler::dumpJson(const char * filename) const
{
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	if (ofs.good())
	{
		dumpJson(ofs);
	}
	else
	{
		SN_ERROR("Couldn't dump profiling data to file '" << filename << '\'');
	}
}

//------------------------------------------------------------------------------
void serialize(JsonBox::Value & o, const Profiler::Sample & sample)
{
	o["file"].setString(sample.file);
	o["line"].setInt(sample.line);
	o["hitCount"].setInt(sample.hitCount);
	o["totalTime"].setInt(sample.totalTime.asMicroseconds());
}

//------------------------------------------------------------------------------
void Profiler::dumpJson(std::ostream & os) const
{
	JsonBox::Value jsonSamples;

	size_t i = 0;
	for (auto it = m_samples.begin(); it != m_samples.end(); ++it)
	{
		const Sample & sample = *it;
		serialize(jsonSamples[i++], sample);
	}

	JsonBox::Value jsonRoot;
	jsonRoot["samples"] = jsonSamples;
	jsonRoot.writeToStream(os);
}

} // namespace sn


