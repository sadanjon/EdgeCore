#include "Window.h"
#include "Events.h"
#include "EventDispatcher.h"
#include "Thread.h"

namespace edge {

#ifdef WIN32

bool Window::m_created = false;

static wchar_t s_wcTitle[256];
static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// window handle
static HWND s_hWnd;

Window::Window(Sint32 x, Sint32 y, 
        Uint32 width, Uint32 height, 
        char *title)  {
    if (m_created) {
        // TODO: no game
    }   
    
    mbstowcs(s_wcTitle, title, strlen(title)); 

    const wchar_t wcName[]  = TEXT("Sample Window Class");
    
    WNDCLASS wc = { };   
    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = g_hInstance;
    wc.lpszClassName = wcName;
    wc.style         = CS_HREDRAW | CS_OWNDC |     // to catch window redraws, unique DC, and resize 
                       CS_VREDRAW;
    //wc.hCursor
    //wc.hIcon

    if (!RegisterClass(&wc)) {
        //TODO: no game
    }

    // Create the window.
    s_hWnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, // Optional window styles.
        wcName,                             // Window class
        s_wcTitle,                            // Window text
        WS_OVERLAPPED | WS_SYSMENU |  
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN,  // Window style

        // Size and position
        x,y, width, height,

        NULL,       // Parent window    
        NULL,       // Menu
        g_hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (s_hWnd == NULL) {
        //TODO: no game
    }

    ShowWindow(s_hWnd, g_nCmdShow);

    m_created = true;
}

Window::~Window(void) {
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
        PostMessage(s_hWnd, WM_USER, NULL, NULL);
    }
}

static Key translateKeyboard(Uint32 virtualKey);

static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static EventDispatcher *ed = EventDispatcher::getInstance();

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    case WM_USER:
        DestroyWindow(s_hWnd);
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
