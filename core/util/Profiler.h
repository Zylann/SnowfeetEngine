#ifndef __HEADER_SN_PROFILER__
#define __HEADER_SN_PROFILER__

#include <vector>
#include <stack>
#include <ostream>
#include <unordered_map>

#include <core/system/time/Clock.hpp>
#include <core/util/macros.hpp>

#ifdef SN_BUILD_NO_PROFILER
	#define SN_PROFILE_LINE // 
#else
	#define SN_PROFILE_LINE
#endif

#define SN_BEGIN_PROFILE_SAMPLE()              SN_PROFILE_LINE sn::Profiler::get().beginSample(SN_FUNCNAME, __FILE__, __LINE__)
#define SN_BEGIN_PROFILE_SAMPLE_NAMED(_name)   SN_PROFILE_LINE sn::Profiler::get().beginSample(SN_FUNCNAME, __FILE__, __LINE__, _name)

#define SN_END_PROFILE_SAMPLE()                SN_PROFILE_LINE sn::Profiler::get().endSample()

#define SN_PROFILE_SCOPED_SAMPLE()             SN_PROFILE_LINE ProfilerScopedSample __sn_profilerScopedSample(SN_FUNCNAME, __FILE__, __LINE__)
#define SN_PROFILE_SCOPED_SAMPLE_NAMED(_name)  SN_PROFILE_LINE ProfilerScopedSample __sn_profilerScopedSample(SN_FUNCNAME, __FILE__, __LINE__, _name)

namespace sn
{

/// \brief Simple profiler for C++ code blocks.
/// \note Not thread-safe yet.
class Profiler
{
public:
	enum DumpMode
	{
		DUMP_JSON
	};

	struct Sample
	{
		const char * name;
		const char * file;
		const char * customName;
		s32 line;
		u32 hitCount;
		Time totalTime;
		Time pausedTime;
		Clock clock;
	};

	static Profiler & get();

	void beginSample(
		const char * funcName, 
		const char * file, 
		s32 line, 
		const char * customName=nullptr
	);

	void endSample();

	void dump(const char * filename, DumpMode mode) const;
	void dump(std::ostream & os, DumpMode mode) const;
	void dumpJson(std::ostream & os) const;

	void setEnabled(bool e);
	bool isEnabled() const { return m_enabled; }

private:
	u32 getParentSample() const;

private:
	bool m_enabled;
	std::vector<Sample> m_samples;
	std::unordered_map<const char *, u32> m_nameToSample;
	std::stack<u32> m_sampleStack;

};

class ProfilerScopedSample
{
public:
	ProfilerScopedSample(
		const char * funcName, 
		const char * file, 
		s32 line, 
		const char * customName = nullptr)
	{
		Profiler::get().beginSample(funcName, file, line, customName);
	}

	~ProfilerScopedSample()
	{
		Profiler::get().endSample();
	}
};


} // namespace sn

#endif // __HEADER_SN_PROFILER__

