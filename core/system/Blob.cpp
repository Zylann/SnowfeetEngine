/*
Blob.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Blob.h"
#include <cstring>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

namespace sn
{

//------------------------------------------------------------------------------
void Blob::append(const void * data, std::size_t sizeInBytes)
{
	if(data && (sizeInBytes > 0))
	{
		std::size_t start = m_data.size();
		m_data.resize(start + sizeInBytes);
		std::memcpy(&m_data[start], data, sizeInBytes);
	}
}

//------------------------------------------------------------------------------
void Blob::clear()
{
	m_data.clear();
	m_readPos = 0;
}

//------------------------------------------------------------------------------
bool Blob::canRead(std::size_t itemLength)
{
	return m_readPos + itemLength <= m_data.size();
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(bool v)
{
	*this << static_cast<u8>(v);
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(u8 v)
{
	append(&v, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(s8 v)
{
	append(&v, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(u16 v)
{
	u16 nv = htons(v);
	append(&nv, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(s16 v)
{
	s16 nv = htons(v);
	append(&nv, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(u32 v)
{
	u32 nv = htonl(v);
	append(&nv, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(s32 v)
{
	s32 nv = htonl(v);
	append(&nv, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(f32 v)
{
	append(&v, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(f64 v)
{
	append(&v, sizeof(v));
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(std::string v)
{
	// First insert string length
	u32 length = static_cast<u32>(v.size());
	*this << length;

	// Then insert characters
	if (length > 0)
		append(v.c_str(), length * sizeof(std::string::value_type));

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator<<(std::wstring v)
{
	// First insert string length
	u32 length = static_cast<u32>(v.size());
	*this << length;

	// Then insert characters
	if (length > 0)
	{
		for (std::wstring::const_iterator c = v.begin(); c != v.end(); ++c)
			*this << static_cast<u32>(*c);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(bool & v)
{
	u8 nv;
	if(canRead(sizeof(nv)))
	{
		*this >> nv;
		v = nv != 0;
	}
	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(u8 & v)
{
	if(canRead(sizeof(v)))
	{
		v = *reinterpret_cast<const u8*>(&m_data[m_readPos]);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(s8 & v)
{
	if(canRead(sizeof(v)))
	{
		v = *reinterpret_cast<const s8*>(&m_data[m_readPos]);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(u16 & v)
{
	if(canRead(sizeof(v)))
	{
		u16 nv = *reinterpret_cast<const u16*>(&m_data[m_readPos]);
		v = ntohs(nv);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(s16 & v)
{
	if(canRead(sizeof(v)))
	{
		s16 nv = *reinterpret_cast<const s16*>(&m_data[m_readPos]);
		v = ntohs(nv);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(u32 & v)
{
	if(canRead(sizeof(v)))
	{
		u32 nv = *reinterpret_cast<const u32*>(&m_data[m_readPos]);
		v = ntohl(nv);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(s32 & v)
{
	if(canRead(sizeof(v)))
	{
		s32 nv = *reinterpret_cast<const s32*>(&m_data[m_readPos]);
		v = ntohl(nv);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(f32 & v)
{
	if(canRead(sizeof(v)))
	{
		v = *reinterpret_cast<const f32*>(&m_data[m_readPos]);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(f64 & v)
{
	if(canRead(sizeof(v)))
	{
		v = *reinterpret_cast<const f64*>(&m_data[m_readPos]);
		m_readPos += sizeof(v);
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(std::string & v)
{
	// First extract string length
	u32 length = 0;
	*this >> length;

	v.clear();
	if ((length > 0) && canRead(length))
	{
		// Then extract characters
		v.assign(&m_data[m_readPos], length);

		// Update reading position
		m_readPos += length;
	}

	return *this;
}

//------------------------------------------------------------------------------
Blob & Blob::operator>>(std::wstring & v)
{
	// First extract string length
	u32 length = 0;
	*this >> length;

	v.clear();
	if((length > 0) && canRead(length * sizeof(u32)))
	{
		// Then extract characters
		for(u32 i = 0; i < length; ++i)
		{
			u32 character = 0;
			*this >> character;
			v += static_cast<wchar_t>(character);
		}
	}

	return *this;
}


} // namespace sn






