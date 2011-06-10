#include "edgeDef.h"
#include "thread.h"
#include "logger.h"
#include "error.h"

namespace edge {

RenderingContext rc = {};

#ifndef NDEBUG
Logger loggerErr("err.txt", true);
#endif

#ifdef WIN32

#define CMDLINE_LENGTH 1024
void createArgv(int &argc, char **&argv) {	
    char *cmdLine = new char[CMDLINE_LENGTH];
	wchar_t *wCmdl = GetCommandLineW();    
    if (wcstombs(cmdLine, wCmdl, wcslen(wCmdl)) < 0) {
        throw Error(ERRID_ERRNO);
    }

    argv = new char*[CMDLINE_LENGTH];
	argv[0] = strtok(cmdLine, " \t");
	argc = 1;	
    char *tok = strtok(NULL, " \t");
    for (int i = 1; tok != NULL; ++i) {
        ++argc;
        argv[i] = tok;
        tok = strtok(NULL, " \t");        
    }	

    delete[] cmdLine;
}
#undef CMDLINE_LENGTH

#endif //WIN32
} // namespace edge