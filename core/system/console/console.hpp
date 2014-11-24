#ifndef __HEADER_SN_CONSOLE__
#define __HEADER_SN_CONSOLE__

namespace sn
{

enum ConsoleColor
{
    SN_CC_DEFAULT,

    SN_CC_DARK_RED,
    SN_CC_DARK_YELLOW,
    SN_CC_DARK_GREEN,
    SN_CC_DARK_BLUE,
    SN_CC_DARK_CYAN,
    SN_CC_DARK_MAGENTA,

    SN_CC_RED,
    SN_CC_YELLOW,
    SN_CC_GREEN,
    SN_CC_BLUE,
    SN_CC_CYAN,
    SN_CC_MAGENTA,

    SN_CC_WHITE,
    SN_CC_GRAY,
    SN_CC_DARKGRAY,
    SN_CC_BLACK
};

ConsoleColor getConsoleForeground();
ConsoleColor getConsoleBackground();

void setConsoleBackground(ConsoleColor color);
void setConsoleForeground(ConsoleColor color);

} // namespace sn

#endif // __HEADER_SN_CONSOLE__

