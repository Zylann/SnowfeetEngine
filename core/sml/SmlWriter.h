#ifndef __HEADER_SML_WRITER__
#define __HEADER_SML_WRITER__

#include <core/util/Variant.h>

namespace sn
{

class SN_API SmlWriter
{
public:
	void writeValue(std::ostream & os, const Variant & value);

private:
	void writeBool(std::ostream & os, bool v);
	void writeString(std::ostream & os, const Variant::String & s);
	void writeArray(std::ostream & os, const Variant::Array & a);
	void writeObject(std::ostream & os, const Variant::Dictionary & o);
	//void writeTypedObject(std::ostream & os, const TypedObject & to);
	void writeSeparator(std::ostream & os);

    bool isWhitespace(char c) const { return !!isspace(c); }

	// TODO Bindings to VariantType::extra
};

} // namespace sn

#endif // __HEADER_SML_WRITER__

