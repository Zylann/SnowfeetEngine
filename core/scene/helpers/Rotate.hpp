#ifndef __HEADER_SN_ROTATE__
#define __HEADER_SN_ROTATE__

#include <core/scene/Entity.hpp>

namespace sn
{

/// \brief Debug helper that makes its parent spin at a specified speed over time.
class Rotate : public Entity
{
public:
	SN_ENTITY(sn::Rotate, sn::Entity)

	Rotate();

	void onUpdate() override;
	void onReady() override;

	void serializeState(JsonBox::Value & o, const SerializationContext & context) override;
	void unserializeState(JsonBox::Value & o, const SerializationContext & context) override;

private:
	Quaternion m_angularSpeed;

};

} // namespace sn

#endif // __HEADER_SN_ROTATE__

