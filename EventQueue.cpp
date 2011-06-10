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
    
    m_cvQueueIsNotEmpty.signal();
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
    static Mutex queueMutex;
    static EventQueue *eq = EventQueue::getInstance();
    queueMutex.lock();

    if (block && eq->size() == 0) {        
        eq->m_cvQueueIsNotEmpty.wait(queueMutex);                
        e = eq->dequeue();   
        return true;
    } 

    if (eq->size() == 0)
        return false;
    
    e = eq->dequeue();   
    return true;
}

} // namespace edge