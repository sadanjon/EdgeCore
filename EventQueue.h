#pragma once

#include <deque>
#include "edgeDef.h"
#include "mutex.h"
#include "events.h"
#include "conditionvariable.h"

namespace edge {


class EventQueue {    
    typedef std::deque<Event> StlEventQueue;
public:
    static EventQueue *getInstance() {
        if (m_instance == NULL)
            m_instance = new EventQueue();

        return m_instance;
    }

    static void kill() {
        if (m_instance != NULL) {
            delete m_instance;
            m_instance = NULL;
        }
    }

    Uint32 size() {
        return m_queue.size();
    }

    void enqueue(Event e);
    Event peek();
    Event dequeue();

private:
    EventQueue();
    ~EventQueue();

    static EventQueue *m_instance;

    StlEventQueue m_queue;
    Mutex m_mutex;
    ConditionVariable m_cvQueueIsNotEmpty;

    friend bool getEvent(Event &e, bool block);
};

// get next event
bool getEvent(Event &e, bool block = false);


} // namespace edge
