#ifndef __HEADER_SN_BEHAVIOUR__
#define __HEADER_SN_BEHAVIOUR__

#include "Component.hpp"

namespace sn
{

// Base class for user-defined gameplay components
class SN_API Behaviour : public Component
{
public:

    SN_OBJECT(sn::Behaviour, sn::Component)

    virtual void onAdd(Entity * e) override;
    virtual void onDestroy() override;

};

} // namespace sn

#endif // __HEADER_SN_BEHAVIOUR__


