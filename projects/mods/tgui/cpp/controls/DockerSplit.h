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

class DockerSplit
{
public:
	DockerSplit(DockerSplit * parent=nullptr, Control * c = nullptr);
	~DockerSplit();

	bool isLeaf() const;
	bool isRoot() const;

	DockPath getPath();

	DockerSplit * getFromPath(const DockPath & path);

	sn::u8 DockerSplit::getIndex();

	void split(Control * dockContent, Direction direction);
	void join(sn::u8 i);

	sn::Vector2i getSize() const;

	void layout(const sn::IntRect & bounds);

	void serializeTree(sn::Variant & o, const sn::SerializationContext & ctx);
	void unserializeTree(const Docker & docker, const sn::Variant & o, const sn::SerializationContext & ctx);

private:
	void setLeaf(Control * c);
	void clearChildren();

private:
	sn::s32 m_position;
	Orientation m_orientation;
	Control * r_control;
	Control * r_sizer;
	DockerSplit * r_parent;
	DockerSplit * m_children[2];
};


} // namespace tgui

#endif // __HEADER_TGUI_DOCKERSPLIT__

