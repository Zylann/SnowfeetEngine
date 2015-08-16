#include "GridLayout.h"
#include "../controls/Control.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
GridLayout::GridLayout(Control & control):
    Layout(control),
    m_columnSpacing(0),
    m_rowSpacing(0),
    m_isConstantRowHeight(false),
    m_constantRowHeight(16)
{
}

//------------------------------------------------------------------------------
void GridLayout::setColumnCount(sn::u32 newCount)
{
    Column defaultColumn;
    defaultColumn.scale = 1.f;
    m_columns.resize(newCount, defaultColumn);
}

//------------------------------------------------------------------------------
void GridLayout::setColumnScale(sn::u32 i, sn::f32 scale)
{
    SN_ASSERT(i < m_columns.size(), "Column index is out of bounds");
    m_columns[i].scale = scale;
}

//------------------------------------------------------------------------------
void GridLayout::setConstantRowHeight(sn::u32 h)
{
    m_constantRowHeight = h;
}

//------------------------------------------------------------------------------
void GridLayout::setIsConstantRowHeight(bool enable)
{
    m_isConstantRowHeight = enable;
}

//------------------------------------------------------------------------------
void GridLayout::update()
{
    // TODO Apply column spacing
    // TODO Apply children margins and anchors

    recalculateColumnSizes();

    std::vector<Control*> children;
    r_control.getChildrenOfType<Control>(children);

    u32 rowCount = children.size() / m_columns.size() + 1;

    u32 childIndex = 0;
    u32 rowY = 0;

    for (u32 rowIndex = 0; rowIndex < rowCount; ++rowIndex)
    {
        // Calculate row height
        u32 rowHeight = m_constantRowHeight;
        if (!m_isConstantRowHeight)
        {
            for (u32 columnIndex = 0; columnIndex < m_columns.size(); ++columnIndex)
            {
                u32 i = childIndex + columnIndex;
                if (i >= children.size())
                    break;

                Control & child = *children[i];
                IntRect childBounds = child.getLocalClientBounds();

                u32 h = static_cast<u32>(childBounds.height());
                if (rowHeight < h)
                    rowHeight = h;
            }
        }

        // Layout row
        for (u32 columnIndex = 0; columnIndex < m_columns.size(); ++columnIndex)
        {
            u32 i = childIndex + columnIndex;
            if (i >= children.size())
                break;

            Control & child = *children[i];
            IntRect childBounds = child.getLocalClientBounds();

            const Column & col = m_columns[columnIndex];

            childBounds.x() = col.x;
            childBounds.y() = rowY;
            childBounds.width() = col.size;
            childBounds.height() = rowHeight;

            child.setLocalClientBounds(childBounds);
            child.layoutChildren();
        }

        childIndex += m_columns.size();
        rowY += rowHeight + m_rowSpacing;
    }
}

//------------------------------------------------------------------------------
void GridLayout::recalculateColumnSizes()
{
    if (m_columns.empty())
        return;

    IntRect clientBounds = r_control.getLocalClientBounds();

    f32 ratio = clientBounds.width();

    // Normalize column scales
    f32 scaleSum = 0;
    for (u32 i = 0; i < m_columns.size(); ++i)
    {
        scaleSum += m_columns[i].scale;
    }
    if (scaleSum > math::ROUNDING_ERROR_F32)
        ratio /= scaleSum;

    s32 x = 0;
    for (u32 i = 0; i < m_columns.size(); ++i)
    {
        Column & col = m_columns[i];
        s32 size = static_cast<s32>(col.scale * ratio);
        col.size = size < 0 ? 0 : static_cast<u32>(size);
    }
    m_columns[0].x = 0;
    for (u32 i = 1; i < m_columns.size(); ++i)
    {
        Column & col1 = m_columns[i-1];
        Column & col2 = m_columns[i];
        col2.x = col1.x + col1.size;
    }
}

//------------------------------------------------------------------------------
void GridLayout::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
    Layout::serializeState(o, ctx);

    sn::serialize(o["columnSpacing"], m_columnSpacing);
    sn::serialize(o["rowSpacing"], m_rowSpacing);
    sn::serialize(o["isConstantRowHeight"], m_isConstantRowHeight);
    sn::serialize(o["constantRowHeight"], m_constantRowHeight);

    if (!m_columns.empty())
    {
        Variant & columnsV = o["columns"];
        columnsV.setArray();
        Variant::Array a = columnsV.getArray();

        a.resize(m_columns.size());
        for (u32 i = 0; i < m_columns.size(); ++i)
        {
            Variant & columnData = a[i];
            Column & column = m_columns[i];

            sn::serialize(columnData["scale"], column.scale);
        }
    }
}

//------------------------------------------------------------------------------
void GridLayout::unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx)
{
    Layout::unserializeState(o, ctx);

    sn::unserialize(o["columnSpacing"], m_columnSpacing);
    sn::unserialize(o["rowSpacing"], m_rowSpacing);
    sn::unserialize(o["isConstantRowHeight"], m_isConstantRowHeight);
    sn::unserialize(o["constantRowHeight"], m_constantRowHeight);

    const Variant & columnsV = o["columns"];
    if (columnsV.isArray())
    {
        const Variant::Array & a = columnsV.getArray();
        setColumnCount(a.size());
        for (size_t i = 0; i < a.size(); ++i)
        {
            const Variant & columnData = a[i];
            Column & column = m_columns[i];

            sn::unserialize(columnData["scale"], column.scale);
        }
    }
}


} // namespace tgui

