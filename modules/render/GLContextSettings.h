#ifndef __HEADER_SNR_GLCONTEXTSETTINGS__
#define __HEADER_SNR_GLCONTEXTSETTINGS__

#include <core/types.h>
#include <cmath>
#include <sstream>

namespace sn
{

struct GLContextSettings
{
    u32 majorVersion;
    u32 minorVersion;
	u32 multiSampleLevel;
	u32 stencilBits;
	u32 depthBits;

    GLContextSettings() :
        majorVersion(3),
        minorVersion(3),
		multiSampleLevel(0),
		stencilBits(8),
		depthBits(32)
    {}

	inline s32 evaluate(u32 bitsPerPixel_, s32 colorBits_, s32 depthBits_, s32 stencilBits_, s32 multiSampleLevel_) const
	{
		return std::abs(static_cast<int>(bitsPerPixel_     - colorBits_))   +
			   std::abs(static_cast<int>(depthBits         - depthBits_))   +
			   std::abs(static_cast<int>(stencilBits       - stencilBits_)) +
			   std::abs(static_cast<int>(multiSampleLevel  - multiSampleLevel_));
	}

	std::string toString() const
	{
		std::stringstream ss;
		ss << "{version=" << majorVersion << "." << minorVersion 
			<< ", multiSampleLevel=" << multiSampleLevel 
			<< ", stencilBits=" << stencilBits 
			<< ", depthBits=" << depthBits << "}";
		return ss.str();
	}
};

inline bool operator!=(const GLContextSettings & lhs, const GLContextSettings & rhs)
{
	return lhs.majorVersion != rhs.majorVersion
		|| lhs.minorVersion != rhs.minorVersion
		|| lhs.multiSampleLevel != rhs.multiSampleLevel
		|| lhs.stencilBits != rhs.stencilBits
		|| lhs.depthBits != rhs.depthBits;
}

} // namespace sn

#endif // __HEADER_SNR_GLCONTEXTSETTINGS__


