#ifndef __HEADER_TGUI_DOCKERSPLIT__
#define __HEADER_TGUI_DOCKERSPLIT__

#include <vector>
#include <core/types.h>
#include <core/math/Rect.h>
#include <core/asset/SerializationContext.h>
#include "../Orientation.h"
#include "../Direction.h"

namespace tgui
{

typedef std::vector<sn::u8> DockPath;

class Docker;
class Control;
class DockSizer;

class DockerSplit
{
public:
	DockerSplit(DockerSplit * parent=nullptr, Control * c = nullptr);
	~DockerSplit();

	bool isLeaf() const;
	bool isRoot() const;

	DockPath getPath(const DockerSplit ** parent=nullptr) const;

	DockerSplit * getFromPath(const DockPath & path);

	void split(Control * dockContent, Direction direction);
	void join(sn::u8 i);

	/// \brief Sets the split position.
	/// \param path: path to the split of which the sizer has to be moved
	/// \param pixelPos: position in pixels relative to the root of the provided path.
	void setSplitPosition(const DockPath & path, sn::s32 pixelPos);

	void layout(const sn::IntRect & bounds);

	void createSizers(Control & container);

	void serializeTree(sn::Variant & o, const sn::SerializationContext & ctx);
	void unserializeTree(const Docker & docker, const sn::Variant & o, const sn::SerializationContext & ctx);

private:
	void setLeaf(Control * c);
	void clearChildren();

	/// \brief Gets the size of the split area containing the control/children.
	/// It is based on the controls, so if layout() has not being performed before,
	/// the returned size will be wrong.
	sn::Vector2i getSize() const;

	sn::u8 DockerSplit::getIndex() const;

	sn::s32 getLocalSplitPosition() const;
	void setLocalSplitPosition(sn::s32 pixelPos);

private:
	sn::f32 m_position;
	Orientation m_orientation;
	Control * r_control;
	DockSizer * r_sizer;
	DockerSplit * r_parent;
	DockerSplit * m_children[2];
};


} // namespace tgui

#endif // __HEADER_TGUI_DOCKERSPLIT__

