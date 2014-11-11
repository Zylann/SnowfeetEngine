#ifndef __HEADER_SN_LAYOUT__
#define __HEADER_SN_LAYOUT__

namespace sn
{

class WindowContainer;

class Layout
{
public:
    Layout(WindowContainer & container):
        r_container(container)
    {}

    virtual ~Layout() {}
    
    virtual void update() = 0;

protected:
    WindowContainer & r_container;

};

} // namespace sn

#endif // __HEADER_SN_LAYOUT__

