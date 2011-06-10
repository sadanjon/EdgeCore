#ifndef DEF_H
#define DEF_H

#include <cstdio>
#include <cstring>

#ifdef WIN32
#include <windows.h>
#endif

namespace edge {

#ifndef NDEBUG
class Logger;
extern Logger loggerErr;
#endif


#ifdef WIN32
typedef INT8 Sint8;
typedef UINT8 Uint8;
typedef INT16 Sint16;
typedef UINT16 Uint16;
typedef INT32 Sint32;
typedef UINT32 Uint32;
typedef INT64 Sint64;
typedef UINT64 Uint64;

typedef PTHREAD_START_ROUTINE ThreadProc;
typedef HANDLE ThreadHandle;
typedef CRITICAL_SECTION MutexHandle;
typedef CONDITION_VARIABLE CVHandle;

#define THREAD DWORD WINAPI

struct RenderingContext {
    HINSTANCE hInstance;
    int nCmdShow;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
};
extern RenderingContext rc;

extern Sint32 g_nCmdShow;

#define main(__int_argc__,__char_argv__) WINAPI WinMain(HINSTANCE hInstance, HINSTANCE h, LPSTR lpszCmdLine, int nCmdShow) {\
	int argc;\
    char **argv;\
	edge::createArgv(argc, argv);\
    edge::rc.hInstance = hInstance;\
    edge::rc.nCmdShow = nCmdShow;\
    int retValue;\
    try {\
	    retValue =  edge::__main(argc, argv);\
    } catch (edge::Error &e) {\
        EG_DBG("Exception caught: %s\n",e.getMessage());\
    }\
    edge::EventQueue::kill();\
    edge::EventDispatcher::kill();\
	delete[] argv;\
	return retValue;\
} int edge::__main(int argc, char **argv)

#endif // WIN32

// create argc and argv
void createArgv(int &argc, char **&argv);

// magic new start symbol
int __main(int argc, char **argv);


struct time_t {
	Uint32 year; 
	Uint32 month; /*1 - 12*/
	Uint32 dayOfWeek; /* 0 - 6, 0 is Sunday */
	Uint32 day; /* 1 - 31 */
	Uint32 hour; /* 0 -23 */
	Uint32 min; /* 0 - 59 */
	Uint32 sec; /* 0 - 59 */
	Uint32 msec; /* 0 - 999 */
};

// get amount of msecs since sometime (os dependant)
Uint64 getTicks_imp();

// gets the date
void getTime(time_t *et);





} // namespace edge

#endif /*DEF_H*/

