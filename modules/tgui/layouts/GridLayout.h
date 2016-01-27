#ifndef __HEADER_TGUI_GRIDLAYOUT__
#define __HEADER_TGUI_GRIDLAYOUT__

#include <modules/tgui/layouts/Layout.h>
#include <vector>

namespace tgui
{

class SN_TGUI_API GridLayout : public Layout
{
public:
	SN_OBJECT

    struct Column
    {
        sn::f32 scale;
        sn::s32 x; // Computed
        sn::s32 size; // Computed
    };

    GridLayout(Control * control = nullptr);

    void setColumnCount(sn::u32 newCount);
    void setColumnScale(sn::u32 i, sn::f32 scale);
    void setConstantRowHeight(sn::u32 h);
    void setIsConstantRowHeight(bool enable);

    const Column & getColumn(sn::u32 i) const { return m_columns[i]; }
    bool isConstantRowHeight() const { return m_isConstantRowHeight; }
    sn::u32 getConstantRowHeight() const { return m_constantRowHeight; }

    void update() override;

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

private:
    void recalculateColumnSizes(const Control & container);

private:
    std::vector<Column> m_columns;
    sn::s32 m_columnSpacing;
    sn::s32 m_rowSpacing;
    bool m_isConstantRowHeight;
    sn::u32 m_constantRowHeight;

};

} // namespace tgui

#endif // __HEADER_TGUI_GRIDLAYOUT__

