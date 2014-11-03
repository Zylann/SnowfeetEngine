#ifndef __HEADER_SN_STRING__
#define __HEADER_SN_STRING__

#include <string>
#include <vector>
#include <core/types.hpp>

namespace sn
{

typedef std::wstring String;

// TODO universal String
//class _String
//{
//public:
//
//	typedef s32 Char;
//
//	_String(const std::string & str);
//	_String(const std::wstring & wstr);
//
//	inline std::size_t size() const { return m_chars.size(); }
//	inline bool empty() const { return m_chars.empty(); }
//	inline void clear() { m_chars.clear(); }
//
//	inline Char & operator[](std::size_t i) { return m_chars[i]; }
//
//	_String & operator=(const std::string & str);
//	_String & operator=(const std::wstring & wstr);
//	_String & operator=(const _String & other);
//
//private:
//
//	std::vector<Char> m_chars;
//};

} // namespace sn

#endif // __HEADER_SN_STRING__

