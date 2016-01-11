#ifndef __HEADER_VOXY_VOXELARRAY__
#define __HEADER_VOXY_VOXELARRAY__

#include "Voxel.h"

namespace voxy
{

template <u32 SIZE_X, u32 SIZE_Y, u32 SIZE_Z>
class VoxelArray
{
public:
	static const u32 VOLUME = SIZE_X * SIZE_Y * SIZE_Z;

	//--------------------------------
	// Constructor / destructor
	//--------------------------------

	VoxelArray()
	{
		memset(m_arrays, 0, sizeof(u8*)*ATTRIB_MAX);
		memset(m_defaultValues, 0, sizeof(u8)*ATTRIB_MAX);
	}

	VoxelArray(const VoxelArray<SIZE_X,SIZE_Y,SIZE_Z> & other)
	{
		for (u32 i = 0; i < ATTRIB_MAX; ++i)
		{
			if (other.m_arrays[i])
				memcpy(m_arrays[i], other.m_arrays[i], sizeof(u8)*VOLUME);
			else
				m_arrays[i] = nullptr;
		}
		memcpy(m_defaultValues, other.m_defaultValues, sizeof(u8)*ATTRIB_MAX);
	}

	~VoxelArray()
	{
		clear();
	}

	//--------------------------------
	// Methods
	//--------------------------------

	u32 sizeX() const { return SIZE_X; }
	u32 sizeY() const { return SIZE_Y; }
	u32 sizeZ() const { return SIZE_Z; }
	u32 volume() const { return VOLUME; }

	u32 getIndex(u32 rx, u32 ry, u32 rz) const { return rx + (ry + rz * SIZE_Y) * SIZE_X; }

	u8 getValueRaw(Attribute a, u32 i) const { return m_arrays[a][i]; }
	void setValueRaw(Attribute a, u32 i, u8 v) { m_arrays[a][i] = v; }

	const u8 * getArray(Attribute a) const { return m_arrays[a]; }

	bool hasAttribute(Attribute a) const { return m_arrays[a] != nullptr; }
	void setAttributeDefaultValue(Attribute a, u8 val) { m_defaultValues[a] = val; };
	
	void setAttribute(Attribute a, sn::u8 fillValue)
	{
		if(!m_arrays[a])
			m_arrays[a] = new u8(VOLUME);
		memset(m_arrays[a], fillValue, v);
	}

	void removeAttribute(Attribute a)
	{
		if(m_arrays[a])
		{
			delete[] m_arrays[a];
			m_arrays[a] = nullptr;
		}
	}

	void clear()
	{
		for (u32 i = 0; i < ATTRIB_MAX; ++i)
		{
			if (m_arrays[i])
				delete[] m_arrays[i];
		}
	}

	void operator=(const VoxelArray<SIZE_X,SIZE_Y,SIZE_Z> & other)
	{
		for (u32 i = 0; i < ATTRIB_MAX; ++i)
		{
			if (other.m_arrays[i])
			{
				if (m_arrays[i] == nullptr)
					m_arrays[i] = new u8[VOLUME];
				memcpy(m_arrays[i], other.m_arrays[i], sizeof(u8)*VOLUME);
			}
			else
			{
				removeAttribute(i);
			}
		}

		memcpy(m_defaultValues, other.m_defaultValues, sizeof(u8)*ATTRIB_MAX);
	}

	template <u32 OTHER_SIZE_X, u32 OTHER_SIZE_Y, u32 OTHER_SIZE_Z>
	void copyFrom(const VoxelArray<OTHER_SIZE_X, OTHER_SIZE_Y, OTHER_SIZE_Z> & other)
	{
		// TODO
	}

private:
	u8 * m_arrays[ATTRIB_MAX];
	u8 m_defaultValues[ATTRIB_MAX];

};

} // namespace voxy

#endif // __HEADER_VOXY_VOXELARRAY__

