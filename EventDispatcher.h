#pragma once

#include "edgeDef.h"
#include "events.h"
#include "EventQueue.h"

namespace edge {

class EventDispatcher {
public:
    static EventDispatcher *getInstance() {
        if (m_instance == NULL)
            m_instance = new EventDispatcher();
        return m_instance;
    }

    static void kill() {
        if (m_instance != NULL) {
            delete m_instance;
            m_instance = NULL;
        }
    }

    void mouseKey(Key key, KeyState keyState);  
    void mouseWheel(Sint32 delta);
    void mouseMove(Sint32 x, Sint32 y);
    void keyboard(Key key, KeyState keyState);    
    void system(SysCode code);

private:
    EventDispatcher();
    ~EventDispatcher();

    static EventDispatcher *m_instance;
    static EventQueue *m_eventQueue;

    Sint32 m_mouseX, m_mouseY;
    Sint32 m_mouseWheel;    
};

} // namespace edge