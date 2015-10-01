#ifndef __HEADER_TGUI_LIST_LAYOUT__
#define __HEADER_TGUI_LIST_LAYOUT__

#include "Layout.h"
#include "../Orientation.h"

namespace tgui
{

/// \brief Positions Control children as a list.
class ListLayout : public Layout
{
public:
	SN_OBJECT(tgui::ListLayout, tgui::Layout)

    ListLayout(Control * control = nullptr);

    void setOrientation(Orientation newOrientation);
    void setSpacing(sn::s32 newSpacing);

    sn::s32 getSpacing() const { return m_spacing; }
    Orientation getOrientation() const { return m_orientation; }

    void update() override;

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

private:
    sn::s32 m_spacing;
    Orientation m_orientation;
	std::unordered_set<sn::u32> m_fillers; // TODO Should use weak refs rather than child indexes

};

} // namespace tgui

#endif // __HEADER_TGUI_LIST_LAYOUT__

