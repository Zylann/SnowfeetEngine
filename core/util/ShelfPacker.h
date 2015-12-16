#ifndef __HEADER_SN_SHELFPACKER__
#define __HEADER_SN_SHELFPACKER__

#include <core/math/Rect.h>
#include <vector>

namespace sn
{

/// \brief Packs rectangles into a rectangular container.
/// Nodes are stored in rows like on a bookshelf.
/// Possible uses are font glyphes or sprite packing on textures.
template <typename T>
class ShelfPacker
{
public:
    //--------------------------------------------------------------------------
    /// \brief Element to store in the rectangular container.
    struct Node
    {
        /// \brief Rectangular bounds of the element.
        /// \note Just set the width and height, the position will be updated by the packer.
        IntRect rect;

        T item;

        Node(T a_item, u32 a_width, u32 a_height):
            item(a_item)
        {
            rect.width() = a_width;
            rect.height() = a_height;
        }

        Node(const Node & other) :
            rect(other.rect),
            item(other.item)
        {}
    };

    //--------------------------------------------------------------------------
    struct Row
    {
        unsigned int y;
        unsigned int width;
        unsigned int height;
        Row() : y(0), width(0), height(0) {}
    };

    //--------------------------------------------------------------------------
    /// \brief Constructs a packer with an initial rectangular container size
    ShelfPacker(u32 initialWidth, u32 initialHeight) :
        m_width(initialWidth),
        m_height(initialHeight)
    {}

    //--------------------------------------------------------------------------
    /// \brief Tries to dynamically insert a new rectangle into the container.
    /// \param n: node to insert
    /// \return false if it doesn't fits given the current layout. 
    /// \note Subsequent calls to this method can lead to an unoptimized packing.
    /// Calling repack() can optimize space if nodes can't be inserted anymore.
    bool insert(Node n, IntRect * out_rect = nullptr)
    {
        if (static_cast<s32>(m_width) < n.rect.width())
        {
            // Can't store this node
            return false;
        }

        Row * row = findRow(n.rect.width(), n.rect.height());

        if (row == nullptr)
        {
            row = createRow(n.rect.height());
        }

        if (row)
        {
            n.rect.x() = row->width;
            n.rect.y() = row->y;
            row->width += n.rect.width();
            m_nodes.push_back(n);

            if (out_rect)
            {
                *out_rect = n.rect;
            }
        }

        return row != nullptr;
    }

    //--------------------------------------------------------------------------
    /// \brief Resizes the rectangular container and recalculates packing for a better fit.
    void repack(u32 width, u32 height)
    {
        m_width = width;
        m_height = height;
        repack();
    }

    //--------------------------------------------------------------------------
    /// \brief Recalculates current packing for a better fit.
    /// This method takes advantage of the fact that nodes are already known, 
    /// so they can be sorted the right way to form optimized rows.
    /// \return false if nodes don't fit
    bool repack()
    {
        std::vector<Row> rows;
        std::vector< Node<T> > nodes = m_nodes;

        // Sort nodes by height (we can do this on the member directly, it doesn't matters)
        std::sort(nodes.begin(), nodes.end(), compareNodesHeight);

        u32 remainingHeight = m_height;

        Row row;
        row.width = m_width;

        // Create rows with increasing heights.
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
        {
            Node<T> & n = *it;

            // If the node doesn't fits horizontally in the row
            if (n.rect.width() > m_width - row.width)
            {
                // If the row contains nodes
                if (!row.nodes.empty())
                {
                    // commit the row to start a new one

                    rows.push_back(row);

                    row.y += row.height;
                    row.width = 0;
                    row.height = 0;
                }
            }

            // If the node fits horizontally in the row
            if (n.rect.width() <= m_width - row.width)
            {
                // If the node doesn't fits vertically
                if (row.height < n.rect.height())
                {
                    // If there is enough space for the row to be resized
                    if (remainingHeight >= n.rect.height())
                    {
                        // Increase the height of the row
                        u32 delta = n.height() - row.height;
                        remainingHeight += delta;
                        row.height = delta;
                    }
                    else
                    {
                        // Cannot store the node, cancel packing
                        return false;
                    }
                }

                // Append node to the row
                n.rect.x() = row.width;
                n.rect.y() = row.y;
                row.width += n.rect.width();
            }
            else
            {
                // The node can't fit, cancel packing
                return false;
            }
        }

        // Commit the last row
        if (!row.width != 0)
        {
            m_rows.push_back(row);
        }

        // Commit final result
        m_rows = rows;
        m_nodes = nodes;

        return true;
    }

private:
    //--------------------------------------------------------------------------
    Row * findRow(unsigned int width, unsigned int height)
    {
        for (auto it = m_rows.begin(); it != m_rows.end(); ++it)
        {
            Row & r = *it;
            if (height <= r.height && m_width - r.width >= width)
            {
                return &r;
            }
        }
        return nullptr;
    }

    //--------------------------------------------------------------------------
    Row * createRow(unsigned int height)
    {
        u32 usedHeight = m_rows.empty() ? 0 : m_rows.back().y + m_rows.back().height;
        if (height <= m_height - usedHeight)
        {
            unsigned int y = usedHeight;
            m_rows.push_back(Row());
            Row & r = m_rows.back();
            r.y = y;
            r.height = height;
            return &r;
        }
        return nullptr;
    }

    //--------------------------------------------------------------------------
    static bool compareNodesHeight(const Node & a, const Node & b)
    {
        return a.rect.height() < b.rect.height();
    }

private:
    //--------------------------------------------------------------------------
    unsigned int m_width;
    unsigned int m_height;
    std::vector<Row> m_rows;
    std::vector<Node> m_nodes;
};

} // namespace sn

#endif // __HEADER_SN_SHELFPACKER__

