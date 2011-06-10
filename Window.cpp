#include <GL\glew.h>
#include <GL\wglew.h>
#include <GL\GL.h>
#include "window.h"
#include "error.h"
#include "Events.h"
#include "EventDispatcher.h"
#include "Thread.h"

namespace edge {

#ifdef WIN32

bool Window::m_created = false;

static wchar_t s_wcTitle[256];
static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


Window::Window(Sint32 x, Sint32 y, 
        Uint32 width, Uint32 height, 
        char *title)  {
    if (m_created) {
        throw Error(ERRID_WINDOW, "Cannot create 2 window objects");        
    }

    PIXELFORMATDESCRIPTOR pfd;
    
    if (mbstowcs(s_wcTitle, title, strlen(title)) < 0) {
        throw Error(ERRID_ERRNO);
    }

    const wchar_t wcName[]  = TEXT("Sample Window Class");
    
    WNDCLASS wc = { };   
    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = rc.hInstance;
    wc.lpszClassName = wcName;
    wc.style         = CS_HREDRAW | CS_OWNDC |     // to catch window redraws, unique DC, and resize 
                       CS_VREDRAW;
    //wc.hCursor
    //wc.hIcon

    if (!RegisterClass(&wc)) {
        throw Error(ERRID_WINDOW, "Window could not register window class");
    }

    // Create a dummy window, DC and RC for glew
    rc.hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, // Optional window styles.
        wcName,                             // Window class
        s_wcTitle,                            // Window text
        WS_OVERLAPPED | WS_SYSMENU |  
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN,  // Window style

        // Size and position
        x,y, width, height,

        NULL,       // Parent window    
        NULL,       // Menu
        rc.hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (rc.hWnd == NULL) {
        throw Error(ERRID_WINDOW, "Window could not create window");
    }
    
    rc.hDC = GetDC(rc.hWnd);    
    if (rc.hDC == 0) {
        throw Error(ERRID_WINDOW, "Window could not get Device Context");
    }

    SetPixelFormat(rc.hDC, 1, &pfd);    
    rc.hRC = wglCreateContext(rc.hDC);
    if (rc.hRC == 0) {
        throw Error(ERRID_WINDOW, "Window could not get Rendering Context");
    }
    wglMakeCurrent(rc.hDC, rc.hRC);    
    
    GLenum err = glewInit();
	if (GLEW_OK != err)	{
        throw Error(ERRID_WINDOW, "Window could not initialize (glew error): %s", glewGetErrorString(err));      
	}

    if (!GLEW_VERSION_2_1 || !WGL_ARB_create_context || !WGL_ARB_pixel_format) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(rc.hRC);
        ReleaseDC(rc.hWnd, rc.hDC);
        DestroyWindow(rc.hWnd);
        throw Error(ERRID_WINDOW, "Window could not initialize (OpenGL requirements not met)");
    }   

    // Destroy dummy window, DC and RC
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(rc.hRC);
    ReleaseDC(rc.hWnd, rc.hDC);
    DestroyWindow(rc.hWnd);

    // Create the real window, DC and RC
    rc.hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, // Optional window styles.
        wcName,                             // Window class
        s_wcTitle,                            // Window text
        WS_OVERLAPPED | WS_SYSMENU |  
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN,  // Window style

        // Size and position
        x,y, width, height,

        NULL,       // Parent window    
        NULL,       // Menu
        rc.hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (rc.hWnd == NULL) {
        throw Error(ERRID_WINDOW, "Window could not create window");
    }

    rc.hDC = GetDC(rc.hWnd);    
    if (rc.hDC == 0) {
        throw Error(ERRID_WINDOW, "Window could not get Device Context");
    }

    int nPixCount = 0;

    // Specify the important attributes we care about
    int pixAttribs[] = { WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
                         WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
                         WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB, // must be HW accelerated
                         WGL_RED_BITS_ARB,       8, // 8 bits of red precision in window
                         WGL_GREEN_BITS_ARB,     8, // 8 bits of green precision in window
                         WGL_BLUE_BITS_ARB,      8, // 8 bits of blue precision in window
                         WGL_DEPTH_BITS_ARB,     16, // 16 bits of depth precision for window
                         WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB, // pf should be RGBA type                         
                         0}; // NULL termination

    // Ask OpenGL to find the most relevant format matching our attribs
    // Only get one format back.
    int pixelFormat, numPixelFormat;
    GLboolean validPF = wglChoosePixelFormatARB(rc.hDC, pixAttribs, NULL, 1, &pixelFormat, (UINT*)&numPixelFormat);

    if(validPF == GL_FALSE) {        
        ReleaseDC(rc.hWnd, rc.hDC);
        DestroyWindow(rc.hWnd);
        throw Error(ERRID_WINDOW, "Window could not find a valid Pixel format");
    }

    // Got a format, now set it as the current one
    SetPixelFormat(rc.hDC, pixelFormat, &pfd );

    GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,  2,
        WGL_CONTEXT_MINOR_VERSION_ARB,  1, 0};

    rc.hRC = wglCreateContextAttribsARB(rc.hDC, NULL, attribs);
    if (rc.hRC == NULL) {
        ReleaseDC(rc.hWnd, rc.hDC);
        DestroyWindow(rc.hWnd);
        throw Error(ERRID_WINDOW, "Window could not create OpenGL 2.1 rendering context");
    }

    wglMakeCurrent(rc.hDC, rc.hRC);  

    ShowWindow(rc.hWnd, rc.nCmdShow);

    m_created = true;
}

Window::~Window(void) {    
    wglMakeCurrent(NULL, NULL);
    if (rc.hRC != NULL)
        wglDeleteContext(rc.hRC);

    if (rc.hDC != NULL && rc.hWnd != NULL)
        ReleaseDC(rc.hWnd, rc.hDC); 

    if (rc.hWnd != NULL)
        DestroyWindow(rc.hWnd);
}

void Window::startSystemLoop() {    
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {        
        TranslateMessage(&msg);
        DispatchMessage(&msg);        
    }

    // wait for all threads to quit
    ThreadList &tl = Thread::getAllThreads();
    for (ThreadList::iterator it = tl.begin(); it != tl.end(); ++it) {
        it->waitForThread();
    }
}

void Window::endSystemLoop() {
    if (m_created) {        
        // inform the windProc to destroy the window
        PostMessage(rc.hWnd, WM_USER, NULL, NULL);
    }
}

static Key translateKeyboard(Uint32 virtualKey);

static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static EventDispatcher *ed = EventDispatcher::getInstance();

    switch (uMsg) {       
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    case WM_USER:
        // user defined message to quit program
        PostQuitMessage(0);     
        /* (we don't DestroyWindow() here since we destroy the window
            with the Window object) */
        break;
    case WM_CLOSE:
        ed->system(SC_CLOSE_WINDOW);
        return 0;
    case WM_SETFOCUS:
        ed->system(SC_FOCUS_ON);
        break;
    case WM_KILLFOCUS:
        ed->system(SC_FOCUS_OFF);
        break;
    case WM_KEYDOWN:
        ed->keyboard(translateKeyboard(wParam), KS_DOWN);
        break;
    case WM_KEYUP:
        ed->keyboard(translateKeyboard(wParam), KS_UP);
        break;
    case WM_LBUTTONDOWN:
        ed->mouseKey(K_LEFTBUTTON, KS_DOWN);
        break;
    case WM_LBUTTONUP:
        ed->mouseKey(K_LEFTBUTTON, KS_UP);
        break;
    case WM_MBUTTONDOWN:
        ed->mouseKey(K_MIDDLEBUTTON, KS_DOWN);
        break;
    case WM_MBUTTONUP:
        ed->mouseKey(K_MIDDLEBUTTON, KS_UP);
        break;
    case WM_RBUTTONDOWN:
        ed->mouseKey(K_RIGHTBUTTON, KS_DOWN);
        break;
    case WM_RBUTTONUP:
        ed->mouseKey(K_RIGHTBUTTON, KS_UP);
        break;
    case WM_MOUSEMOVE:
        // mouse moves inside window, x and y relative to upper left client rect
        ed->mouseMove(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:
        ed->mouseWheel(HIWORD(wParam));
        break;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static Key translateKeyboard(Uint32 virtualKey) {
    switch (virtualKey) {
        case VK_SPACE:
            return K_SPACEBAR;
        case VK_RETURN:
            return K_ENTER;
        case VK_LEFT:
            return K_LEFT;
        case VK_UP:
            return K_UP;
        case VK_RIGHT:
            return K_RIGHT;
        case VK_DOWN:
            return K_DOWN;
        case VK_LWIN:
            return K_SUPER;
        case VK_RWIN:
            return K_SUPER;                    
        case VK_SHIFT:
            return K_SHIFT;
        case VK_CONTROL:
            return K_CTRL;
        case VK_MENU:
            return K_ALT;
    }

    if (virtualKey >= '0' && virtualKey <= '9') {
        return static_cast<Key>((virtualKey - '0') + K_0);
    } else if (virtualKey >= 'A' && virtualKey <= 'Z') {
        return static_cast<Key>((virtualKey - 'A') + K_A);
    } else {
        return K_NON;
    }
}

#endif // WIN32

} // namespace edge
