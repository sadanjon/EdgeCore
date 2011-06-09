#pragma once

#include "edgeDef.h"
#include "mutex.h"

namespace edge {

class ConditionVariable {
public:
    ConditionVariable();
    ~ConditionVariable();

    /*
    wait until signalled
    mutex must be locked
    */
    void wait(Mutex &m);

    // signal a single thread waiting on CV
    void signal();

    // signal all threads waiting on CV
    void signalAll();

private:
    CVHandle m_handle;
};

} // namespace edge