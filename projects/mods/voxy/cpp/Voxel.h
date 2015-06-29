#ifndef __HEADER_VOXY_VOXEL__
#define __HEADER_VOXY_VOXEL__

#include <core/types.h>

namespace voxy
{

typedef sn::u8 u8;
typedef sn::u32 u32;

enum Attribute
{
	ATTRIB_TYPE = 0,
	ATTRIB_STATE,
	ATTRIB_COLOR,
	ATTRIB_ISOLEVEL,

	ATTRIB_MAX // Keep last
};

} // namespace voxy

#endif // __HEADER_VOXY_VOXEL__

