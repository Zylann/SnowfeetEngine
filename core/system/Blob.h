/*
Blob.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_BLOB__
#define __HEADER_SN_BLOB__

#include <string>
#include <vector>
#include <core/types.h>

namespace sn
{

// Binary stream with network big/little endian conversion support
class SN_API Blob
{
public:

	Blob():
		m_readPos(0)
	{}

	void append(const void* data, std::size_t sizeInBytes);

	void clear();

	inline const void* getData() const { return m_data.empty() ? &m_data[0] : nullptr; }
	inline std::size_t getDataSize() const { return m_data.size(); }

	inline bool endOfRead() const { return m_readPos >= m_data.size(); }

	Blob & operator<<(bool v);
	Blob & operator<<(u8 v);
	Blob & operator<<(s8 v);
	Blob & operator<<(u16 v);
	Blob & operator<<(s16 v);
	Blob & operator<<(u32 v);
	Blob & operator<<(s32 v);
	Blob & operator<<(f32 v);
	Blob & operator<<(f64 v);
	Blob & operator<<(std::string v);
	Blob & operator<<(std::wstring v);

	Blob & operator>>(bool & v);
	Blob & operator>>(u8 & v);
	Blob & operator>>(s8 & v);
	Blob & operator>>(u16 & v);
	Blob & operator>>(s16 & v);
	Blob & operator>>(u32 & v);
	Blob & operator>>(s32 & v);
	Blob & operator>>(f32 & v);
	Blob & operator>>(f64 & v);
	Blob & operator>>(std::string & v);
	Blob & operator>>(std::wstring & v);

private:

	bool canRead(std::size_t itemLength);

	std::vector<char> m_data;
	std::size_t m_readPos;

};

} // namespace sn

#endif // __HEADER_SN_BLOB__




