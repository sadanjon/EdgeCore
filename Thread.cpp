#include "Thread.h"

namespace edge {


#ifdef WIN32

ThreadList Thread::m_threadList;

Thread::Thread(ThreadProc threadProc) : 
    m_handle(NULL), m_started(false) {
    m_threadProc = threadProc;

    m_threadList.push_back(*this);
}

Thread::~Thread(void) {
    if (m_started) {
        CloseHandle(m_handle);   
    }
}

void Thread::start(void *argument) {
    m_handle = CreateThread(NULL, 0, m_threadProc, argument, 0, NULL);
    m_started = true;
}

void Thread::waitForThread() {
    WaitForSingleObject(m_handle, INFINITE);
}

void exitThread() {
    ExitThread(NULL);
}

ThreadList &Thread::getAllThreads() {
    return m_threadList;
}

#endif // WIN32


} // namespace edge