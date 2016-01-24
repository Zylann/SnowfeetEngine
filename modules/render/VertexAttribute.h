#ifndef __HEADER_SN_RENDER_VERTEXATTRIBUTE__
#define __HEADER_SN_RENDER_VERTEXATTRIBUTE__

#include <core/types.h>
#include <core/util/assert.h>
#include <string>

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
	u32 use;

	/// \brief Offset in bytes of the attribute within the vertex structure
	/// (this is only relevant if the layout is an array of structs)
	u32 offset;

	/// \brief Index in attributes.
	/// (only relevant if the layout is arrays of attributes)
	u32 index;

	/// \brief Name of the attribute
	std::string name;

    VertexAttribute():
        count(0)
    {}

    bool isValid() const { return count != 0; }

    u32 sizeBytes() const
    {
        switch (type)
        {
        case TYPE_INT8:
        case TYPE_UINT8:
            return count;
        case TYPE_INT16:
        case TYPE_UINT16:
            return 2 * count;
        case TYPE_INT32:
        case TYPE_UINT32:
        case TYPE_FLOAT32:
            return 4 * count;
        case TYPE_FLOAT64:
            return 8 * count;
        default:
            SN_ASSERT(false, "Invalid state");
            return 0;
        }
    }

};

std::string toString(VertexAttribute::Use use);

} // namespace sn

#endif // __HEADER_SN_RENDER_VERTEXATTRIBUTE__

