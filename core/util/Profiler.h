#ifndef __HEADER_SN_CPPPROFILER__
#define __HEADER_SN_CPPPROFILER__

#include <vector>
#include <stack>
#include <ostream>
#include <unordered_map>

#include <core/system/time/Clock.hpp>

#ifdef SN_BUILD_NO_PROFILER
	#define SN_BEGIN_PROFILE_SAMPLE
	#define SN_END_PROFILE_SAMPLE
#else
	#define SN_BEGIN_PROFILE_SAMPLE(_name) sn::Profiler::get().beginSample(_name, __FILE__, __LINE__)
	#define SN_END_PROFILE_SAMPLE() sn::Profiler::get().endSample()
#endif

namespace sn
{

/// \brief Simple profiler for C++ code blocks.
/// \note Not thread-safe yet.
class Profiler
{
public:
	static Profiler & get();

	void beginSample(const char * name, const char * file, s32 line);
	void endSample();

	void dumpJson(const char * filename) const;
	void dumpJson(std::ostream & os) const;

	void setEnabled(bool e);
	bool isEnabled() const { return m_enabled; }

	struct Sample
	{
		const char * name;
		const char * file;
		s32 line;
		u32 hitCount;
		Time totalTime;
		Clock clock;
	};

private:
	bool m_enabled;
	std::vector<Sample> m_samples;
	std::unordered_map<const char *, u32> m_nameToSample;
	std::stack<u32> m_sampleStack;

};

} // namespace sn

#endif // __HEADER_SN_CPPPROFILER__

