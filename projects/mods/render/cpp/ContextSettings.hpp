#ifndef __HEADER_SNR_CONTEXTSETTINGS__
#define __HEADER_SNR_CONTEXTSETTINGS__

#include <core/types.hpp>
#include <cmath>

namespace sn {
namespace render {

struct ContextSettings
{
    u32 majorVersion;
    u32 minorVersion;
	u32 multiSampleLevel;
	u32 stencilBits;
	u32 depthBits;

    ContextSettings() :
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

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_CONTEXTSETTINGS__


