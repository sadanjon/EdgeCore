#include "EventDispatcher.h"

namespace edge {

EventDispatcher *EventDispatcher::m_instance = NULL;
EventQueue *EventDispatcher::m_eventQueue = EventQueue::getInstance();

EventDispatcher::EventDispatcher() {
}

EventDispatcher::~EventDispatcher() {
}

void EventDispatcher::mouseKey(Key key, KeyState keyState) {
    Event e;
    e.type = ET_MOUSEKEY;
    e.mouse.key = key;
    e.mouse.keyState = keyState;
    e.mouse.x = m_mouseX;
    e.mouse.y = m_mouseY;

    m_eventQueue->enqueue(e);    
}
void EventDispatcher::mouseWheel(Sint32 delta) {
    m_mouseWheel += delta;
    if (delta <= -120 || delta >= 120) {
        Event e;
        e.type = ET_MOUSEKEY;
        e.mouse.key = K_WHEEL;
        if (delta >= 120)
            e.mouse.keyState = KS_UP;
        else
            e.mouse.keyState = KS_DOWN;

        e.mouse.x = m_mouseX;
        e.mouse.y = m_mouseY;

        m_eventQueue->enqueue(e);
        m_mouseWheel = 0;
    }
}
void EventDispatcher::mouseMove(Sint32 x, Sint32 y) {    
    Event e;
    e.type = ET_MOUSEMOVE;
    e.mouse.key = K_NON;
    e.mouse.keyState = KS_NON;
    e.mouse.x = x;
    e.mouse.y = y;

    m_eventQueue->enqueue(e);

    m_mouseX = x;
    m_mouseY = y;    
}
void EventDispatcher::keyboard(Key key, KeyState keyState) {
    Event e;
    e.type = ET_KEYBOARD;
    e.keyboard.key = key;
    e.keyboard.keyState = keyState;
    
    m_eventQueue->enqueue(e);    
}

void EventDispatcher::system(SysCode code) {
    Event e;
    e.type = ET_SYSTEM;
    e.system.code = code;
    
    m_eventQueue->enqueue(e);   
}

} // namespace edge