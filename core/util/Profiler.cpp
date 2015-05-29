#include "Profiler.h"
#include "Log.hpp"
#include "../json/json_utils.hpp"
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
void serialize(JsonBox::Value & o, const Profiler::Sample & sample)
{
	o["file"].setString(sample.file);
	o["line"].setInt(sample.line);
	o["depth"].setInt(sample.depth);
	o["beginTime"].setInt(static_cast<s32>(sample.beginTime.asMicroseconds())); // JSON love
	o["endTime"].setInt(static_cast<s32>(sample.endTime.asMicroseconds()));
	o["overheadTime"].setInt(static_cast<s32>(sample.overheadTime.asMicroseconds()));

	if (sample.name)
		o["name"].setString(sample.name);

	if (sample.customName)
		o["customName"].setString(sample.customName);
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
	jsonRoot.writeToStream(os, false);
}

} // namespace sn


