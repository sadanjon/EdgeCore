#include "Mutex.h"

namespace edge {

#ifdef WIN32
/*
Windows has a "mutex" object, but I use a similar one
called "critical section". The difference is that 
a critical section can be used in only one process.
While this can limit the user of this library somewhat, 
the benefits are that the library can also simulate posix 
CVs correctly (as is available on linux and OSX)
*/
Mutex::Mutex(void) {
    InitializeCriticalSection(&m_handle);   
}

Mutex::~Mutex(void) {
    DeleteCriticalSection(&m_handle);    
}

void Mutex::lock() {
    EnterCriticalSection(&m_handle);    
}

void Mutex::unlock() {
    LeaveCriticalSection(&m_handle);
}
#endif // WIN32

} // namespace edge