#include "VertexDescription.h"

namespace sn
{
	namespace
	{
		struct TypeDesc
		{
			u32 size;
			std::string name;
		};

		TypeDesc s_types[VertexAttribute::TYPE_COUNT];

		void s_init()
		{
			static bool done = false;
			if (done)
				return;
			done = true;

			s_types[VertexAttribute::TYPE_UINT8] = { 1, "uint8" };
			s_types[VertexAttribute::TYPE_UINT16] = { 2, "uint16" };
			s_types[VertexAttribute::TYPE_UINT32] = { 4, "uint32" };

			s_types[VertexAttribute::TYPE_INT8] = { 1, "int8" };
			s_types[VertexAttribute::TYPE_INT16] = { 2, "int16" };
			s_types[VertexAttribute::TYPE_INT32] = { 4, "int32" };

			s_types[VertexAttribute::TYPE_FLOAT32] = { 4, "float32" };
			s_types[VertexAttribute::TYPE_FLOAT64] = { 8, "float64" };
		}

		const TypeDesc & getType(VertexAttribute::Type t)
		{
			s_init();
			return s_types[t];
		}
	}

void VertexDescription::addAttribute(const char * name, VertexAttribute::Use use, VertexAttribute::Type type, u32 elemCount)
{
	SN_ASSERT(elemCount > 0, "An attribute must have at least 1 element");
	
	VertexAttribute a = {
		type,
		elemCount,
		use,
		m_attributes.empty() ? 0 : m_attributes.back().offset + elemCount * getType(type).size,
		m_attributes.size(),
		name
	};

	m_attributes.push_back(a);
}

} // namespace sn

