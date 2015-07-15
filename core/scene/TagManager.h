#ifndef __HEADER_SN_TAGMANAGER__
#define __HEADER_SN_TAGMANAGER__

#include <core/types.h>
#include <vector>
#include <unordered_map>
#include <string>

namespace sn
{

// TODO Compile-time tag count?
/// \brief Maps strings to containers of objects, also providing a zero-based, auto-incrementing index for each tag.
/// This is useful to store tags in an object as a lightweight bitset instead of a set of strings.
template <class T>
class TagManager
{
public:
	static const u32 INVALID_INDEX = -1;

	//--------------------------------------------------------------------------
	u32 add(const std::string & tagName, T e)
	{
		u32 tagIndex;
		auto it = m_tagNameToIndex.find(tagName);
		if(it == m_tagNameToIndex.end())
		{
			tagIndex = m_indexToBucket.size();
			m_tagNameToIndex[tagName] = tagIndex;
			Bucket b;
			b.tagIndex = tagIndex;
			b.tagName = tagName;
			m_indexToBucket.push_back(b);
		}
		else
		{
			tagIndex = it->second;
		}

		Bucket & b = m_indexToBucket[tagIndex];
		b.objects.insert(e);

		return tagIndex;
	}

	//--------------------------------------------------------------------------
	bool remove(u32 tagIndex, T e)
	{
		SN_ASSERT(tagIndex < m_indexToBucket.size(), "Invalid tag index");
		Bucket & b = m_indexToBucket[tagIndex];
		return b.objects.erase(e) != 0;
	}

	//--------------------------------------------------------------------------
	u32 getTagIndex(const std::string & tagName) const
	{
		auto it = m_tagNameToIndex.find(tagName);
		if(it != m_tagNameToIndex.end())
			return it->second;
		return INVALID_INDEX;
	}

	//--------------------------------------------------------------------------
	inline bool tagExists(u32 tagIndex)
	{
		return tagIndex < m_indexToBucket.size();
	}

	//--------------------------------------------------------------------------
	bool getTagName(u32 tagIndex, std::string & out_name) const
	{
		if(tagIndex < m_indexToBucket.size())
		{
			out_name = m_indexToBucket[tagIndex].tagName;
			return true;
		}
		else
		{
			return false;
		}
	}

	//--------------------------------------------------------------------------
	const std::unordered_set<T> & getObjectsByTag(u32 tagIndex)
	{
		SN_ASSERT(tagIndex < m_indexToBucket.size(), "Invalid tag index");
		return m_indexToBucket[tagIndex].objects;
	}

	//--------------------------------------------------------------------------
	void clear()
	{
		m_tagNameToIndex.clear();
		m_indexToBucket.clear();
	}

	//--------------------------------------------------------------------------
	bool isObjectTagged(T object, const std::string & tagName)
	{
		u32 tagIndex = getTagIndex(tagName);
		if (tagIndex == INVALID_INDEX)
			return false;
		Bucket & bucket = m_indexToBucket[tagIndex];
		return bucket.objects.find(object) != bucket.objects.end();
	}

private:
	struct Bucket
	{
		u32 tagIndex;
		std::string tagName;
		std::unordered_set<T> objects;
	};

	std::unordered_map<std::string, u32> m_tagNameToIndex;
	std::vector<Bucket> m_indexToBucket;

};

} // namespace sn

#endif // __HEADER_SN_TAGMANAGER__

