#ifndef __HEADER_SN_LAYOUT__
#define __HEADER_SN_LAYOUT__

namespace sn
{

class Window;

class Layout
{
public:
    Layout(Window * container):
        r_container(container)
    {}

    virtual ~Layout() {}
    
    virtual void update() = 0;

protected:
    Window * r_container;

};

} // namespace sn

#endif // __HEADER_SN_LAYOUT__

