#pragma once

#include <list>
#include "edgeDef.h"


namespace edge {
class Thread;

typedef std::list<Thread> ThreadList;

class Thread {
public:
    Thread(ThreadProc threadProc);

    // does not terminate the thread!
    // use waitForThread()
    ~Thread();

    void start(void *argument);
    void waitForThread();    

    static ThreadList &getAllThreads();

private:
    ThreadHandle m_handle;
    ThreadProc m_threadProc;    
    bool m_started;

    static ThreadList m_threadList;
};

// exits calling thread
void exitThread();

} // namespace edge