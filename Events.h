#pragma once

#include "edgeDef.h"

namespace edge {

enum EventType {
    ET_KEYBOARD, ET_MOUSEKEY, ET_MOUSEMOVE, ET_SYSTEM, ET_USER
};

enum KeyState {
    KS_UP, KS_DOWN, KS_NON
};

// must keep the order of A-Z and 0-9
enum Key {
    K_A,K_B,K_C,K_D,K_E,K_F,K_G,K_H,K_I,K_J,K_K,K_L,K_M,K_N,K_O,K_P,K_Q,K_R,K_S,K_U,K_V,K_W,K_X,K_Y,K_Z,
    K_0,K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9,    
    K_ALT, K_CTRL, K_SUPER, K_SHIFT, K_ENTER,
    K_BACKSPACE, K_SPACEBAR, K_ESC, K_UP, K_RIGHT, K_DOWN, K_LEFT, K_LEFTBUTTON, K_RIGHTBUTTON, K_MIDDLEBUTTON,
    K_WHEEL, K_NON
};

enum SysCode {
    SC_FOCUS_ON, SC_FOCUS_OFF, SC_CLOSE_WINDOW
};

struct EventKeyboard {
    Key key;
    KeyState keyState;
};

struct EventMouse {
    Key key;
    KeyState keyState;
    Sint32 x;
    Sint32 y;
};

struct EventSystem {
    SysCode code;
};

struct EventUser {
    Uint32 code;
    void *data1;
    void *data2;
};

struct Event {
    EventType type;
    union {      
        EventKeyboard keyboard;
        EventMouse mouse;
        EventSystem system;
        EventUser user;
    };
};

} // namespace edge