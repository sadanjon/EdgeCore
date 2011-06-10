#pragma once

#include <cerrno>
#include "edgeDef.h"
#include "logger.h"

namespace edge {

enum ErrorId {
    ERRID_NON, ERRID_CV, ERRID_ERRNO, ERRID_LOGGER, ERRID_ASSERT, ERRID_THREAD, ERRID_WINDOW
};

class Error {
    static const int MESSAGE_SIZE = 256;
public:
    Error(ErrorId id, const char *message, ...);
    Error(ErrorId id = ERRID_NON);
    ~Error();

    const char *getMessage() const {
        return m_msg;
    }

    ErrorId getId() const {
        return m_id;
    }
private:
    char m_msg[MESSAGE_SIZE];
    ErrorId m_id;

};

#ifndef NDEBUG
#define EG_ASSERT(condition, message, ...) \
    if (!(condition)) {\
        edge::loggerErr.write("%s:%d: (%s) failed:\n\t", #condition, __FILE__, __LINE__);\
        edge::loggerErr.write(message, __VA_ARGS__);\
        edge::loggerErr.write("\n");\
    }
#define EG_DBG(message, ...) \
    edge::loggerErr.write("%s:%d:\n\t", __FILE__, __LINE__);\
    edge::loggerErr.write(message, __VA_ARGS__);\
    edge::loggerErr.write("\n");
#else
#define EG_ASSERT (void)0
#define EG_DBG (void)0
#endif



} // namespace edge