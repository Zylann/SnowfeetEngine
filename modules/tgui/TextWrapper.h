#ifndef __HEADER_TGUI_TEXTWRAPPER__
#define __HEADER_TGUI_TEXTWRAPPER__

#include <core/asset/base/Font.h>
#include <core/util/Variant.h>

#include "TextModel.h"

namespace tgui
{

enum TextWrapMode
{
    TGUI_WRAP_NONE,
    TGUI_WRAP_WORDS,
    TGUI_WRAP_CHARACTERS
};

void serialize(sn::Variant & o, TextWrapMode m);
void unserialize(const sn::Variant & o, TextWrapMode & m);

/// \brief Stores a view of a TextModel with a line wrapping algorithm.
/// Text edition and display should use this view rather than TextModel directly.
/// This class refers to a TextModel but will not modify it.
/// One wrap = a line or a part of it
class TextWrapper
{
public:
    //---------------------------------
    // Structures
    //---------------------------------

    struct LightWrap
    {
        /// \brief Index of the line in TextModel
        sn::u32 line;
        /// \brief Index of the post-final character in this wrap.
        /// In the last wrap, it's equal to string.size().
        sn::u32 end;
    };

    struct Wrap
    {
        sn::u32 line;
        sn::u32 begin;
        sn::u32 end;
    };

    //---------------------------------
    // Constructor and options
    //---------------------------------

    TextWrapper(const TextModel & model) :
        m_wrapMode(TGUI_WRAP_WORDS),
        r_model(model)
    {}

    /// \brief Sets the wrapping mode. Doesn't updates the lines.
    void setWrapMode(TextWrapMode m) { m_wrapMode = m; }

    /// \brief Gets the wrapping mode.
    TextWrapMode getWrapMode() const { return m_wrapMode; }

    //---------------------------------
    // State update
    //---------------------------------

    /// \brief Recalculate wraps without... wrapping.
    void updateNoWrap();

    /// \brief Executes wrapping on all lines.
    /// \note: prefer using specialized methods rather than updating the whole text,
    /// it often results in better performances.
    void update(
        sn::u32 width,
        const sn::Font & font,
        const sn::FontFormat & format
    );

    // TODO Per-case functions for more efficiency

    //---------------------------------
    // State access
    //---------------------------------

    /// \brief Gets a row's info as it is internally stored.
    const LightWrap & getLightWrap(sn::u32 visualRow) const;

    /// \brief Gets a row's complete info
    Wrap getWrap(sn::u32 visualRow) const;

    /// \brief Gets the number of visual rows (line count after wrapping)
    sn::u32 getWrapCount() const { return m_wraps.size(); }

    /// \brief Gets the TextModel this wrapper reads
    const TextModel & getTextModel() const { return r_model; }

    /// \brief Gets the first wrap corresponding to the given line
    sn::u32 getWrapFromLine(sn::u32 lineIndex) const;

    sn::u32 getWrapFromLineAndColumn(sn::u32 row, sn::u32 column) const;

    /// \brief Gets the last wrap corresponding to the same line as the given wrap
    sn::u32 getEndWrap(sn::u32 beginWrap) const;

private:
    /// \brief Current wrap mode
    TextWrapMode m_wrapMode;

    /// \brief Source text to wrap
    const TextModel & r_model;

    /// \brief Rows of text obtained after wrapping. One row corresponds to a part of a line.
    std::vector<LightWrap> m_wraps;

};

} // namespace tgui

#endif // __HEADER_TGUI_TEXTWRAPPER__


