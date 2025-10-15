#include <Nova/Nova.hpp>
#include <Flux/Flux.hpp>
#include <iostream>

namespace Nova {
Window::Window(std::string title, int width, int height) {
    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        Flux::Error("Unable to open X display");
        exit(1);
    }
    
    screen = DefaultScreen(display);
    X11Window root = RootWindow(display, screen);
    
    window = XCreateSimpleWindow(display, root, 100, 100, width, height, 1,
                                  BlackPixel(display, screen), 
                                  BlackPixel(display, screen));
    
    XStoreName(display, window, title.c_str());
    
    // Register WM_DELETE_WINDOW protocol
    Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);
    
    // Select input events
    XSelectInput(display, window, 
                 ExposureMask | KeyPressMask | StructureNotifyMask | 
                 ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    
    // Set size hints to prevent resizing
    XSizeHints size_hints;
    size_hints.flags = PMinSize | PMaxSize;
    size_hints.min_width = size_hints.max_width = width;
    size_hints.min_height = size_hints.max_height = height;
    
    XSetWMNormalHints(display, window, &size_hints);
    
    XMapWindow(display, window);
    XFlush(display); // Important: ensure window is created before WebGPU init
    
    backend = Backend::X11;
    
    X11PlatformData* data = new X11PlatformData();
    data->display = display;
    data->window = window;
    platformData = data;
}

bool Window::PollEvents() {
    while (XPending(display) > 0) {  // Changed to 'while' to process all events
        XEvent event;
        XNextEvent(display, &event);
        
        switch (event.type) {
            case KeyPress:
                if (XLookupKeysym(&event.xkey, 0) == XK_Escape) {
                    return false;
                }
                break;
                
            case ClientMessage: {
                Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
                if (static_cast<Atom>(event.xclient.data.l[0]) == WM_DELETE_WINDOW) {
                    return false;
                }
                break;
            }
            
            case ConfigureNotify:
                // Handle window configuration changes if needed
                break;
        }
    }
    return true;
}
}