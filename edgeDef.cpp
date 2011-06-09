#include "edgeDef.h"
#include "thread.h"

namespace edge {

#ifdef WIN32

HINSTANCE g_hInstance;
HDC  g_hDC;
HGLRC g_hGLRC;
Sint32 g_nCmdShow;

#define CMDLINE_LENGTH 1024
void createArgv(int &argc, char **&argv) {	
    char *cmdLine = new char[CMDLINE_LENGTH];
	wchar_t *wCmdl = GetCommandLineW();    
	wcstombs(cmdLine, wCmdl, wcslen(wCmdl));

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