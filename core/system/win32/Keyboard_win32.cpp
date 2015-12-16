#include "Keyboard_win32.h"

namespace sn {

bool isKeyPressed(KeyCode key)
{
    return HIWORD(GetAsyncKeyState(win32::genericKeyCodeToPlatform(key))) != 0;
}

namespace win32 {

    // win32 specific

    namespace keyboard
    {
        const u32 MAX_KEYS = 0xFF;

        KeyCode g_platformKeyToGeneric[MAX_KEYS] = { SN_KEY_UNKNOWN };
        s32 g_genericKeyToPlatform[MAX_KEYS] = { 0 };

        void defineKey(s32 platformCode, KeyCode genericCode)
        {
            g_platformKeyToGeneric[platformCode] = genericCode;
            g_genericKeyToPlatform[genericCode] = platformCode;
        }

        void checkGlobalKeyboardInit()
        {
            static bool initialized = false;
            if (initialized)
                return;
            else
                initialized = true;

            defineKey(VK_LWIN,        SN_KEY_LSYSTEM);
            defineKey(VK_RWIN,        SN_KEY_RSYSTEM);
            defineKey(VK_APPS,        SN_KEY_MENU);
            defineKey(VK_OEM_1,       SN_KEY_SEMICOLON);
            defineKey(VK_OEM_2,       SN_KEY_SLASH);
            defineKey(VK_OEM_PLUS,    SN_KEY_EQUAL);
            defineKey(VK_OEM_MINUS,   SN_KEY_DASH);
            defineKey(VK_OEM_4,       SN_KEY_LBRACKET);
            defineKey(VK_OEM_6,       SN_KEY_RBRACKET);
            defineKey(VK_OEM_COMMA,   SN_KEY_COMMA);
            defineKey(VK_OEM_PERIOD,  SN_KEY_PERIOD);
            defineKey(VK_OEM_7,       SN_KEY_QUOTE);
            defineKey(VK_OEM_5,       SN_KEY_BACKSLASH);
            defineKey(VK_OEM_3,       SN_KEY_TILDE);
            defineKey(VK_ESCAPE,      SN_KEY_ESCAPE);
            defineKey(VK_SPACE,       SN_KEY_SPACE);
            defineKey(VK_RETURN,      SN_KEY_RETURN);
            defineKey(VK_BACK,        SN_KEY_BACKSPACE);
            defineKey(VK_TAB,         SN_KEY_TAB);
            defineKey(VK_PRIOR,       SN_KEY_PAGEUP);
            defineKey(VK_NEXT,        SN_KEY_PAGEDOWN);
            defineKey(VK_END,         SN_KEY_END);
            defineKey(VK_HOME,        SN_KEY_HOME);
            defineKey(VK_INSERT,      SN_KEY_INSERT);
            defineKey(VK_DELETE,      SN_KEY_DELETE);
            defineKey(VK_ADD,         SN_KEY_ADD);
            defineKey(VK_SUBTRACT,    SN_KEY_SUBTRACT);
            defineKey(VK_MULTIPLY,    SN_KEY_MULTIPLY);
            defineKey(VK_DIVIDE,      SN_KEY_DIVIDE);
            defineKey(VK_PAUSE,       SN_KEY_PAUSE);
                                      
            defineKey(VK_F1,          SN_KEY_F1);
            defineKey(VK_F2,          SN_KEY_F2);
            defineKey(VK_F3,          SN_KEY_F3);
            defineKey(VK_F4,          SN_KEY_F4);
            defineKey(VK_F5,          SN_KEY_F5);
            defineKey(VK_F6,          SN_KEY_F6);
            defineKey(VK_F7,          SN_KEY_F7);
            defineKey(VK_F8,          SN_KEY_F8);
            defineKey(VK_F9,          SN_KEY_F9);
            defineKey(VK_F10,         SN_KEY_F10);
            defineKey(VK_F11,         SN_KEY_F11);
            defineKey(VK_F12,         SN_KEY_F12);
            defineKey(VK_F13,         SN_KEY_F13);
            defineKey(VK_F14,         SN_KEY_F14);
            defineKey(VK_F15,         SN_KEY_F15);            
            // Can go up to F24       
                                      
            defineKey(VK_LEFT,        SN_KEY_LEFT);
            defineKey(VK_RIGHT,       SN_KEY_RIGHT);
            defineKey(VK_UP,          SN_KEY_UP);
            defineKey(VK_DOWN,        SN_KEY_DOWN);
                                      
            defineKey(VK_NUMPAD0,     SN_KEY_PAD0);
            defineKey(VK_NUMPAD1,     SN_KEY_PAD1);
            defineKey(VK_NUMPAD2,     SN_KEY_PAD2);
            defineKey(VK_NUMPAD3,     SN_KEY_PAD3);
            defineKey(VK_NUMPAD4,     SN_KEY_PAD4);
            defineKey(VK_NUMPAD5,     SN_KEY_PAD5);
            defineKey(VK_NUMPAD6,     SN_KEY_PAD6);
            defineKey(VK_NUMPAD7,     SN_KEY_PAD7);
            defineKey(VK_NUMPAD8,     SN_KEY_PAD8);
            defineKey(VK_NUMPAD9,     SN_KEY_PAD9);
                                      
            defineKey('A',            SN_KEY_A);
            defineKey('Z',            SN_KEY_Z);
            defineKey('E',            SN_KEY_E);
            defineKey('R',            SN_KEY_R);
            defineKey('T',            SN_KEY_T);
            defineKey('Y',            SN_KEY_Y);
            defineKey('U',            SN_KEY_U);
            defineKey('I',            SN_KEY_I);
            defineKey('O',            SN_KEY_O);
            defineKey('P',            SN_KEY_P);
            defineKey('Q',            SN_KEY_Q);
            defineKey('S',            SN_KEY_S);
            defineKey('D',            SN_KEY_D);
            defineKey('F',            SN_KEY_F);
            defineKey('G',            SN_KEY_G);
            defineKey('H',            SN_KEY_H);
            defineKey('J',            SN_KEY_J);
            defineKey('K',            SN_KEY_K);
            defineKey('L',            SN_KEY_L);
            defineKey('M',            SN_KEY_M);
            defineKey('W',            SN_KEY_W);
            defineKey('X',            SN_KEY_X);
            defineKey('C',            SN_KEY_C);
            defineKey('V',            SN_KEY_V);
            defineKey('B',            SN_KEY_B);
            defineKey('N',            SN_KEY_N);

            defineKey('0',            SN_KEY_0);
            defineKey('1',            SN_KEY_1);
            defineKey('2',            SN_KEY_2);
            defineKey('3',            SN_KEY_3);
            defineKey('4',            SN_KEY_4);
            defineKey('5',            SN_KEY_5);
            defineKey('6',            SN_KEY_6);
            defineKey('7',            SN_KEY_7);
            defineKey('8',            SN_KEY_8);
            defineKey('9',            SN_KEY_9);
        }

    } // keyboard


    int genericKeyCodeToPlatform(KeyCode key)
    {
        keyboard::checkGlobalKeyboardInit();
        return keyboard::g_genericKeyToPlatform[key];
    }

    KeyCode platformKeyCodeToGeneric(WPARAM key, LPARAM flags)
    {
        keyboard::checkGlobalKeyboardInit();

        switch (key)
        {
            // Check the scancode to distinguish between left and right shift
        case VK_SHIFT:
        {
            static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
            return scancode == lShift ? SN_KEY_LSHIFT : SN_KEY_RSHIFT;
        }

            // Check the "extended" flag to distinguish between left and right alt
        case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? SN_KEY_RALT : SN_KEY_LALT;

            // Check the "extended" flag to distinguish between left and right control
        case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? SN_KEY_RCONTROL : SN_KEY_LCONTROL;

        default:
            if (key >= 0 && key < keyboard::MAX_KEYS)
                return keyboard::g_platformKeyToGeneric[key];
            else
                return SN_KEY_UNKNOWN;
        }
    }

} // namespace win32
} // namespace sn


