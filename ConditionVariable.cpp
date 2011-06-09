#include "ConditionVariable.h"

namespace edge {

#ifdef WIN32

ConditionVariable::ConditionVariable(void) {    
    // no initialization required in windows   
}

ConditionVariable::~ConditionVariable(void) {
    // and no freeing
}

void ConditionVariable::wait(Mutex &m) {
    SleepConditionVariableCS(&m_handle, &m.m_handle, INFINITE);    
}

void ConditionVariable::signal() {
    WakeConditionVariable(&m_handle);
}

void ConditionVariable::signalAll() {
    WakeAllConditionVariable(&m_handle);
}

#endif // WIN32

} // namespace edge