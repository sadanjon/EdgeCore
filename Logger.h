#pragma once

#include <cstdio>
#include "mutex.h"

namespace edge {

class Logger {
public:
    Logger(const char *filename, bool immidiate = false);
    ~Logger();

    void write(const char *format, ...);
private:
    FILE *m_file;
    Mutex m_loggerMutex;
};

} //namespace edge