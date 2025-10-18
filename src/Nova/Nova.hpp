#pragma once
#include <wayland-client.h>
#include <wayland-egl.h>
#include <Flux/Flux.hpp>
#include <Nova/Key.hpp>
#include <string>
#include <stack>
#include <unordered_map>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using X11Window = ::Window;

namespace Nova
{
    // Helper function to convert X11 KeySym to Nova::Key
    static Key X11KeySymToNovaKey(KeySym keysym)
    {
        switch (keysym)
        {
        case XK_a:
        case XK_A:
            return Key::KEY_A;
        case XK_b:
        case XK_B:
            return Key::KEY_B;
        case XK_c:
        case XK_C:
            return Key::KEY_C;
        case XK_d:
        case XK_D:
            return Key::KEY_D;
        case XK_e:
        case XK_E:
            return Key::KEY_E;
        case XK_f:
        case XK_F:
            return Key::KEY_F;
        case XK_g:
        case XK_G:
            return Key::KEY_G;
        case XK_h:
        case XK_H:
            return Key::KEY_H;
        case XK_i:
        case XK_I:
            return Key::KEY_I;
        case XK_j:
        case XK_J:
            return Key::KEY_J;
        case XK_k:
        case XK_K:
            return Key::KEY_K;
        case XK_l:
        case XK_L:
            return Key::KEY_L;
        case XK_m:
        case XK_M:
            return Key::KEY_M;
        case XK_n:
        case XK_N:
            return Key::KEY_N;
        case XK_o:
        case XK_O:
            return Key::KEY_O;
        case XK_p:
        case XK_P:
            return Key::KEY_P;
        case XK_q:
        case XK_Q:
            return Key::KEY_Q;
        case XK_r:
        case XK_R:
            return Key::KEY_R;
        case XK_s:
        case XK_S:
            return Key::KEY_S;
        case XK_t:
        case XK_T:
            return Key::KEY_T;
        case XK_u:
        case XK_U:
            return Key::KEY_U;
        case XK_v:
        case XK_V:
            return Key::KEY_V;
        case XK_w:
        case XK_W:
            return Key::KEY_W;
        case XK_x:
        case XK_X:
            return Key::KEY_X;
        case XK_y:
        case XK_Y:
            return Key::KEY_Y;
        case XK_z:
        case XK_Z:
            return Key::KEY_Z;

        case XK_1:
            return Key::KEY__1;
        case XK_2:
            return Key::KEY__2;
        case XK_3:
            return Key::KEY__3;
        case XK_4:
            return Key::KEY__4;
        case XK_5:
            return Key::KEY__5;
        case XK_6:
            return Key::KEY__6;
        case XK_7:
            return Key::KEY__7;
        case XK_8:
            return Key::KEY__8;
        case XK_9:
            return Key::KEY__9;
        case XK_0:
            return Key::KEY__0;

        case XK_Return:
            return Key::KEY_RETURN;
        case XK_Escape:
            return Key::KEY_ESCAPE;
        case XK_BackSpace:
            return Key::KEY_BACKSPACE;
        case XK_Tab:
            return Key::KEY_TAB;
        case XK_space:
            return Key::KEY_SPACE;
        case XK_minus:
            return Key::KEY_MINUS;
        case XK_equal:
            return Key::KEY_EQUALS;
        case XK_bracketleft:
            return Key::KEY_LEFTBRACKET;
        case XK_bracketright:
            return Key::KEY_RIGHTBRACKET;
        case XK_backslash:
            return Key::KEY_BACKSLASH;
        case XK_semicolon:
            return Key::KEY_SEMICOLON;
        case XK_apostrophe:
            return Key::KEY_APOSTROPHE;
        case XK_grave:
            return Key::KEY_GRAVE;
        case XK_comma:
            return Key::KEY_COMMA;
        case XK_period:
            return Key::KEY_PERIOD;
        case XK_slash:
            return Key::KEY_SLASH;
        case XK_Caps_Lock:
            return Key::KEY_CAPSLOCK;

        case XK_F1:
            return Key::KEY_F1;
        case XK_F2:
            return Key::KEY_F2;
        case XK_F3:
            return Key::KEY_F3;
        case XK_F4:
            return Key::KEY_F4;
        case XK_F5:
            return Key::KEY_F5;
        case XK_F6:
            return Key::KEY_F6;
        case XK_F7:
            return Key::KEY_F7;
        case XK_F8:
            return Key::KEY_F8;
        case XK_F9:
            return Key::KEY_F9;
        case XK_F10:
            return Key::KEY_F10;
        case XK_F11:
            return Key::KEY_F11;
        case XK_F12:
            return Key::KEY_F12;

        case XK_Print:
            return Key::KEY_PRINTSCREEN;
        case XK_Scroll_Lock:
            return Key::KEY_SCROLLLOCK;
        case XK_Pause:
            return Key::KEY_PAUSE;
        case XK_Insert:
            return Key::KEY_INSERT;
        case XK_Home:
            return Key::KEY_HOME;
        case XK_Page_Up:
            return Key::KEY_PAGEUP;
        case XK_Delete:
            return Key::KEY_DELETE;
        case XK_End:
            return Key::KEY_END;
        case XK_Page_Down:
            return Key::KEY_PAGEDOWN;
        case XK_Right:
            return Key::KEY_RIGHT;
        case XK_Left:
            return Key::KEY_LEFT;
        case XK_Down:
            return Key::KEY_DOWN;
        case XK_Up:
            return Key::KEY_UP;

        case XK_Num_Lock:
            return Key::KEY_NUMLOCKCLEAR;
        case XK_KP_Divide:
            return Key::KEY_KP_DIVIDE;
        case XK_KP_Multiply:
            return Key::KEY_KP_MULTIPLY;
        case XK_KP_Subtract:
            return Key::KEY_KP_MINUS;
        case XK_KP_Add:
            return Key::KEY_KP_PLUS;
        case XK_KP_Enter:
            return Key::KEY_KP_ENTER;
        case XK_KP_1:
            return Key::KEY_KP_1;
        case XK_KP_2:
            return Key::KEY_KP_2;
        case XK_KP_3:
            return Key::KEY_KP_3;
        case XK_KP_4:
            return Key::KEY_KP_4;
        case XK_KP_5:
            return Key::KEY_KP_5;
        case XK_KP_6:
            return Key::KEY_KP_6;
        case XK_KP_7:
            return Key::KEY_KP_7;
        case XK_KP_8:
            return Key::KEY_KP_8;
        case XK_KP_9:
            return Key::KEY_KP_9;
        case XK_KP_0:
            return Key::KEY_KP_0;
        case XK_KP_Decimal:
            return Key::KEY_KP_PERIOD;

        case XK_Control_L:
            return Key::KEY_LCTRL;
        case XK_Shift_L:
            return Key::KEY_LSHIFT;
        case XK_Alt_L:
            return Key::KEY_LALT;
        case XK_Super_L:
            return Key::KEY_LGUI;
        case XK_Control_R:
            return Key::KEY_RCTRL;
        case XK_Shift_R:
            return Key::KEY_RSHIFT;
        case XK_Alt_R:
            return Key::KEY_RALT;
        case XK_Super_R:
            return Key::KEY_RGUI;

        default:
            return Key::KEY_UNKNOWN;
        }
    }

    class PlatformData
    {
    public:
        virtual ~PlatformData() = default;
    };

    class X11PlatformData : public PlatformData
    {
    public:
        Display *display;
        X11Window window;
    };

    enum class Backend
    {
        Wayland,
        X11
    };

    enum class MouseButton
    {
        Left,
        Right,
        Middle
    };

    class Event
    {
    public:
        virtual ~Event() = default;
    };

    class MouseMoveEvent : public Event
    {
    public:
        int x = 0;
        int y = 0;
    };

    class MouseButtonDownEvent : public Event
    {
    public:
        MouseButton button;
    };

    class MouseButtonUpEvent : public Event
    {
    public:
        MouseButton button;
    };

    class KeyDownEvent : public Event
    {
    public:
        Key key;
        bool shift = false;
    };

    class KeyUpEvent : public Event
    {
    public:
        Key key;
        bool shift = false;
    };

    class Window
    {
    private:
        Display *display;
        X11Window window;
        int screen;
        bool shift;

        std::stack<Event *> eventStack;

        std::unordered_map<Key, bool> keyStates;

        bool cursorLocked = false;
        Cursor invisibleCursor;


    public:
        PlatformData *platformData;
        Backend backend;

        int width, height;

        Window(std::string title, int width, int height);

        bool PollEvents();

        bool HasEvents();

        Event *PopEvent();

        void LockCursor();
        void UnlockCursor();
    };
}