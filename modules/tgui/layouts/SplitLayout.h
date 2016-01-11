#ifndef __HEADER_TGUI_SPLITLAYOUT__
#define __HEADER_TGUI_SPLITLAYOUT__

#include <vector>
#include <core/types.h>
#include <core/math/Rect.h>
#include <core/asset/SerializationContext.h>
#include "../Orientation.h"
#include "../Direction.h"
#include "Layout.h"

namespace tgui
{

typedef std::vector<sn::u8> DockPath;

class Docker;
class Control;
class DockSizer;

/// \brief Layouts controls in splits of two.
/// Splits can be nested to form a tree of splits, while controls will still have the same parent.
/// It can be used in docking systems.
class SplitLayout : public Layout
{
public:
	SN_OBJECT

	SplitLayout(Control * container = nullptr, SplitLayout * parent = nullptr, Control * c = nullptr);
	~SplitLayout();

	//-----------------------------------------------------------
	// Layout
	//-----------------------------------------------------------

	void update() override;

	void onReady() override;
	void onControlRemoved(const Control & c) override;

	void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
	void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

	//-----------------------------------------------------------
	// SplitLayout
	//-----------------------------------------------------------

	bool isLeaf() const;
	bool isRoot() const;

	DockPath getPath(const SplitLayout ** parent=nullptr) const;

	SplitLayout * getFromPath(const DockPath & path);

	void split(Control * dockContent, Direction direction);
	void join(sn::u8 i);

	/// \brief Sets the split position.
	/// \param path: path to the split of which the sizer has to be moved
	/// \param pixelPos: position in pixels relative to the root of the provided path.
	void setSplitPosition(const DockPath & path, sn::s32 pixelPos);

	void createSizers(Control & container);

private:
	void layout(const sn::IntRect & bounds);

	void setLeaf(Control * c);
	void clearChildren();

	/// \brief Gets the size of the split area containing the control/children.
	/// It is based on the controls, so if layout() has not being performed before,
	/// the returned size will be wrong.
	sn::Vector2i getSize() const;

	sn::u8 SplitLayout::getIndex() const;

	sn::s32 getLocalSplitPosition() const;
	void setLocalSplitPosition(sn::s32 pixelPos);

	void serializeTree(sn::Variant & o, const sn::SerializationContext & ctx);
	void unserializeTree(Control & container, const sn::Variant & o, const sn::SerializationContext & ctx);

private:
	sn::f32 m_position;
	Orientation m_orientation;
	Control * r_control;
	DockSizer * r_sizer;
	SplitLayout * r_parent;
	SplitLayout * m_children[2];
};


} // namespace tgui

#endif // __HEADER_TGUI_SPLITLAYOUT__

