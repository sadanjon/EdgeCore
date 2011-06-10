#include "ConditionVariable.h"
#include "error.h"

namespace edge {

#ifdef WIN32

ConditionVariable::ConditionVariable(void) {        
    InitializeConditionVariable(&m_handle);
}

ConditionVariable::~ConditionVariable(void) {
    // no freeing nessecery in windows
}

void ConditionVariable::wait(Mutex &m) {
    if (!SleepConditionVariableCS(&m_handle, &m.m_handle, INFINITE)) {
        throw Error(ERRID_CV, "Thread failed to sleep on CV");
    }
}

void ConditionVariable::signal() {
    WakeConditionVariable(&m_handle);
}

void ConditionVariable::signalAll() {
    WakeAllConditionVariable(&m_handle);
}

#endif // WIN32

} // namespace edge