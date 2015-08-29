#ifndef __HEADER_TGUI_LAYOUT__
#define __HEADER_TGUI_LAYOUT__

#include <core/types.h>
#include <core/util/Variant.h>
#include <core/asset/SerializationContext.h>
#include <core/reflect/Object.h>

namespace tgui
{

class Control;

/// \brief Controls how children items are positionned inside their parent
class Layout : public sn::Object
{
public:
	SN_OBJECT(tgui::Layout, sn::Object)

	Layout(Control * container = nullptr) : r_container(container) {}

    virtual void update() = 0;
	virtual void onReady() {};

    virtual void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) {};
    virtual void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) {};

protected:
	friend class Control;

	Control * getContainer() const { return r_container; }
	void setContainer(Control & c) { r_container = &c; }

private:
	Control * r_container;

};

} // namespace tgui

#endif // __HEADER_TGUI_LAYOUT__

