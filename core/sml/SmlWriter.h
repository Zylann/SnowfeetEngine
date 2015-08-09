#ifndef __HEADER_SML_WRITER__
#define __HEADER_SML_WRITER__

#include <core/util/Variant.h>

namespace sn
{

/// \brief Writes SML byte stream from a Variant.
class SN_API SmlWriter
{
public:
    SmlWriter(bool pretty=false);

    bool isPretty() const { return m_pretty; }
    void setPretty(bool pretty) { m_pretty = pretty; }

	void writeValue(std::ostream & os, const Variant & value);

private:
	void writeBool(std::ostream & os, bool v);
	void writeString(std::ostream & os, const Variant::String & s);
	void writeArray(std::ostream & os, const Variant::Array & a);
	void writeObject(std::ostream & os, const Variant::Dictionary & o);
	//void writeTypedObject(std::ostream & os, const TypedObject & to);
	void writeSeparator(std::ostream & os);

    bool isWhitespace(char c) const { return !!isspace(c); }

    void writeIndent(std::ostream & os);

    void openBlock(std::ostream & os, char c);
    void closeBlock(std::ostream & os, char c);

	// TODO Bindings to VariantType::extra

private:
    bool m_pretty;
    u32 m_level;
    std::string m_indent;

};

} // namespace sn

#endif // __HEADER_SML_WRITER__

