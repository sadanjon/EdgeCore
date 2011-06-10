#include "Logger.h"
#include "error.h"

namespace edge {

Logger::Logger(const char *filename, bool immidiate) {
    m_file = fopen(filename, "w");
    if (m_file == NULL) {                
        throw Error(ERRID_LOGGER, "Logger could not open file \"%s\" for writing", filename);
    }

    if (immidiate) {
        // disable buffering
        setvbuf(m_file, NULL, _IONBF, 0);
    }
}

Logger::~Logger() {
    fclose(m_file);
}

void Logger::write(const char *format, ...) {
    va_list args;
    va_start(args, format);
    m_loggerMutex.lock();
    
    vfprintf(m_file, format, args);
    
    m_loggerMutex.unlock();
    va_end(args);
}

} //namespace edge