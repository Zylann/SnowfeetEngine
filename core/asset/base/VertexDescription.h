#ifndef __HEADER_SN_VERTEXDESCRIPTION__
#define __HEADER_SN_VERTEXDESCRIPTION__

#include <core/types.hpp>
#include <core/util/assert.hpp>
#include <string>
#include <vector>

namespace sn
{

struct VertexAttribute
{
	enum Type
	{
		TYPE_UINT8 = 0,
		TYPE_UINT16,
		TYPE_UINT32,

		TYPE_INT8,
		TYPE_INT16,
		TYPE_INT32,

		TYPE_FLOAT32,
		TYPE_FLOAT64,

		TYPE_COUNT // Keep last
	};

	enum Use
	{
		USE_POSITION = 0,
		USE_COLOR,
		USE_TEXCOORD,
		USE_NORMAL,

		// Reserved space...

		USE_CUSTOM = 10,

		// User space...
	};

	/// \brief Type of the elements in the attribute
	Type type;

	/// \brief Number of elements in the attribute.
	/// 1 = numbers, 2 = 2D vectors, etc
	u32 count;

	/// \brief What is the use of this attribute
	Use use;

	/// \brief Offset in bytes of the attribute within the vertex structure
	/// (this is only relevant if the layout is an array of structs)
	u32 offset;

	/// \brief Index in attributes.
	/// (only relevant if the layout is arrays of attributes)
	u32 index;

	/// \brief Debug name of the attribute
	std::string debugName;
};

class VertexDescription
{
public:
	enum Layout
	{
		LAYOUT_STRUCTS,
		LAYOUT_ARRAYS
	};

	typedef std::vector<VertexAttribute> AttributeList;

	const AttributeList & getAttributes() const { return m_attributes; }

	void addAttribute(const char * name, VertexAttribute::Use use, VertexAttribute::Type type, u32 elemCount = 1);

private:
	AttributeList m_attributes;
	Layout m_layout;

};

} // namespace sn

#endif // __HEADER_SN_VERTEXDESCRIPTION__

