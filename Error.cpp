#include <cstring>
#include <cstdarg>
#include "Error.h"

namespace edge {

Error::Error(ErrorId id) {
    switch (id) {
    case ERRID_NON:
        strncpy(m_msg, "Unspecified error", MESSAGE_SIZE);    
        break;
    case ERRID_CV:
        strncpy(m_msg, "ConditionVariable error", MESSAGE_SIZE);    
        break;
    case ERRID_ERRNO:
        sprintf(m_msg, "Errno: %s", strerror(errno));
        break;
    case ERRID_LOGGER: 
        sprintf(m_msg, "Logger error");
        break;
    case ERRID_ASSERT: 
        sprintf(m_msg, "Assert error");
        break;
    case ERRID_THREAD: 
        sprintf(m_msg, "Thread error");
        break;
    case ERRID_WINDOW:
        sprintf(m_msg, "Window error");
        break;
    default:
        *m_msg = 0;
        break;
    }
}

Error::Error(ErrorId id, const char *message, ...) {
    va_list args;
    va_start(args, message);

    vsprintf(m_msg, message, args); 
    strncpy(m_msg, message, MESSAGE_SIZE);    
    m_id = id;

    va_end(args);
}

Error::~Error(void) {
}

} // namespace edge