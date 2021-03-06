/*
console.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_CONSOLE__
#define __HEADER_SN_CONSOLE__

#include <core/config.h>

namespace sn
{

enum ConsoleColor
{
	SN_CC_BLACK = 0,
	SN_CC_DARKGRAY,
	SN_CC_GRAY,
	SN_CC_WHITE,

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

	SN_CC_DEFAULT,
};

ConsoleColor getConsoleForeground();
ConsoleColor getConsoleBackground();

void SN_API setConsoleBackground(ConsoleColor color);
void SN_API setConsoleForeground(ConsoleColor color);

void SN_API pauseConsole();

} // namespace sn

#endif // __HEADER_SN_CONSOLE__

