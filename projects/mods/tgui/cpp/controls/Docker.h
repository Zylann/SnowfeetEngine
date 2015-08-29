#ifndef __HEADER_TGUI_DOCKER__
#define __HEADER_TGUI_DOCKER__

#include "DockerSplit.h"
#include "Control.h"

namespace tgui
{

class Docker : public Control
{
public:
    SN_ENTITY(tgui::Docker, tgui::Control)

    void layoutChildren() override;
	DockerSplit * getDockLayout() { return &m_split; }

	void onReady() override;

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

private:
	void createSizers();

private:
    DockerSplit m_split;

};

} // namespace tgui

#endif // __HEADER_TGUI_DOCKER__

