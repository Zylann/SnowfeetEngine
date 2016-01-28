#ifndef __HEADER_SN_VERTEXDESCRIPTION__
#define __HEADER_SN_VERTEXDESCRIPTION__

#include <core/util/assert.h>
#include <vector>

#include <modules/render/VertexAttribute.h>
#include <modules/render/common.h>

namespace sn
{

typedef std::vector<VertexAttribute> VertexAttributeList;

class SN_RENDER_API VertexDescription
{
public:
	//enum Layout
	//{
	//	LAYOUT_STRUCTS,
	//	LAYOUT_ARRAYS
	//};

	const VertexAttributeList & getAttributes() const { return m_attributes; }

	void addAttribute(const std::string name, u32 use, VertexAttribute::Type type, u32 elemCount = 1);

    const VertexAttribute * getAttributeByUse(u32 use) const
    {
        for (u32 i = 0; i < m_attributes.size(); ++i)
        {
            if (m_attributes[i].use == use)
                return &m_attributes[i];
        }
        return nullptr;
    }

private:
	VertexAttributeList m_attributes;
	//Layout m_layout;

};

} // namespace sn

#endif // __HEADER_SN_VERTEXDESCRIPTION__

