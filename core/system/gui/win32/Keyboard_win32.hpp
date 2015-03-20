#ifndef __HEADER_SN_KEYBOARD_WIN32__
#define __HEADER_SN_KEYBOARD_WIN32__

#include "../Keyboard.hpp"
#include <core/types.hpp>
#include <Windows.h>

namespace sn {
namespace win32 {

KeyCode platformKeyCodeToGeneric(WPARAM key, LPARAM flags);
int genericKeyCodeToPlatform(KeyCode keyCode);

} // namespace win32
} // namespace sn

#endif // __HEADER_SN_KEYBOARD_WIN32__

