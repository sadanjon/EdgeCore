#include "EventQueue.h"


namespace edge {

EventQueue *EventQueue::m_instance = NULL;

EventQueue::EventQueue() {
}

EventQueue::~EventQueue() {
    while (m_queue.size() > 0)
        m_queue.pop_back();
}

void EventQueue::enqueue(Event e) {
    m_mutex.lock();
    
    m_cvQueueIsNotEmpty.signalAll();
    m_queue.push_front(e);

    m_mutex.unlock();
}

Event EventQueue::peek() {
    m_mutex.lock();

    Event ret = m_queue.back();

    m_mutex.unlock();
    return ret;
}

Event EventQueue::dequeue() {
    m_mutex.lock();

    Event ret = m_queue.back();
    m_queue.pop_back();

    m_mutex.unlock();
    return ret;
}


// get next event
bool getEvent(Event &e, bool block) {    
    static EventQueue *eq = EventQueue::getInstance();    
    eq->m_mutex.lock();
    if (block) {            
        if (eq->m_queue.size() != 0) {            
            e = eq->m_queue.back();
            eq->m_queue.pop_back();
            eq->m_mutex.unlock();
            // (mutex unlocked from within dequeue)
            return true;
        } else {
            eq->m_cvQueueIsNotEmpty.wait(eq->m_mutex);                
            e = eq->m_queue.back();
            eq->m_queue.pop_back();
            eq->m_mutex.unlock();
            return true;
        }
    } else { 
        if (eq->m_queue.size() == 0) {
            eq->m_mutex.unlock();
            return false;
        } else {
            e = eq->m_queue.back();
            eq->m_queue.pop_back();
            eq->m_mutex.unlock();
            return true;
        }
    }
}

} // namespace edge