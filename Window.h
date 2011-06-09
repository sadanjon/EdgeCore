#pragma once

#include "edgeDef.h"

namespace edge {

class Window {        
public:
    // windowed constructor
    Window(Sint32 x, Sint32 y, 
        Uint32 width, Uint32 height, 
        char *title);

    // fullscreen contructor
    Window(Uint32 xResulotion, Uint32 yResolution,
        char *title);

    ~Window();

    void startSystemLoop();    
    void endSystemLoop();

private:    
    static bool m_created;  

};

} // namespace edge