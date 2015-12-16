#include "helpers_win32.h"
#include <Windows.h>
#include <tchar.h>

namespace sn {
namespace win32 {

std::string getLastError()
{
    std::string lastError = _T("");
    DWORD errorCode = ::GetLastError();
    if (errorCode != 0)
    {
        LPSTR messageBuffer = NULL;
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&messageBuffer,
            0,
            NULL
        );
#ifdef UNICODE
        // TODO Implement this function...
        Win32String message = Win32_AnsiToUnicode(messageBuffer);
#else
        std::string message(messageBuffer, size);
#endif
        lastError = message;
        LocalFree(messageBuffer);
    }
    return lastError;
}

} // namespace win32
} // namespace sn

