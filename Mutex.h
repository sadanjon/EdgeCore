#pragma once

#include "edgeDef.h"

namespace edge {

class ConditionVariable;

class Mutex {
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

private:
    MutexHandle m_handle;

    friend class ConditionVariable;
};

} // namespace edge