#include <Nova/Nova.hpp>
#include <Flux/Flux.hpp>
#include <iostream>

namespace Nova
{
    Window::Window(std::string title, int width, int height)
    {
        display = XOpenDisplay(nullptr);
        if (display == nullptr)
        {
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
             ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
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

        X11PlatformData *data = new X11PlatformData();
        data->display = display;
        data->window = window;
        platformData = data;

        this->width = width;
        this->height = height;
    }

    bool Window::PollEvents()
    {
        if (!eventStack.empty())
        {
            Flux::Error("Not all events handled!");
        }

        while (XPending(display) > 0)
        { // Changed to 'while' to process all events
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type)
            {
            case KeyPress:
            {
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                Key key = X11KeySymToNovaKey(keysym);

                if (key == Key::KEY_LSHIFT || key == Key::KEY_RSHIFT) {
                    shift = true;
                }

                // If key is not already marked as pressed, it's a real press
                if (key != Key::KEY_UNKNOWN && !keyStates[key])
                {
                    KeyDownEvent *keyDownEvent = new KeyDownEvent();
                    keyDownEvent->key = key;
                    keyDownEvent->shift = shift;
                    eventStack.push(keyDownEvent);
                    keyStates[key] = true;
                }
                break;
            }

            case KeyRelease:
            {
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                Key key = X11KeySymToNovaKey(keysym);

                if (key == Key::KEY_LSHIFT || key == Key::KEY_RSHIFT) {
                    shift = false;
                }

                // Use XEventsQueued + Peek to filter out auto-repeat releases
                if (XEventsQueued(display, QueuedAfterReading))
                {
                    XEvent next_event;
                    XPeekEvent(display, &next_event);

                    if (next_event.type == KeyPress &&
                        next_event.xkey.keycode == event.xkey.keycode &&
                        next_event.xkey.time == event.xkey.time)
                    {
                        // Auto-repeat event, skip this release
                        break;
                    }
                }

                if (key != Key::KEY_UNKNOWN && keyStates[key])
                {
                    KeyUpEvent *keyUpEvent = new KeyUpEvent();
                    keyUpEvent->key = key;
                    keyUpEvent->shift = shift;
                    eventStack.push(keyUpEvent);
                    keyStates[key] = false;
                }
                break;
            }

            case MotionNotify:
            {
                if (cursorLocked)
                {
                    int centerX = width / 2;
                    int centerY = height / 2;

                    int dx = event.xmotion.x - centerX;
                    int dy = event.xmotion.y - centerY;

                    if (dx != 0 || dy != 0)
                    {
                        // Push delta movement event
                        MouseMoveEvent* mouseMoveEvent = new MouseMoveEvent();
                        mouseMoveEvent->x = dx;
                        mouseMoveEvent->y = dy;
                        eventStack.push(mouseMoveEvent);

                        // Warp back to center
                        XWarpPointer(display, None, window, 0, 0, 0, 0, centerX, centerY);
                        XFlush(display);
                    }
                }
                else
                {
                    MouseMoveEvent *mouseMoveEvent = new MouseMoveEvent();
                    mouseMoveEvent->x = event.xmotion.x;
                    mouseMoveEvent->y = event.xmotion.y;
                    eventStack.push(mouseMoveEvent);
                }

                break;
            }


            case ButtonPress:
            {
                MouseButtonDownEvent *mouseDownEvent = new MouseButtonDownEvent();

                if (event.xbutton.button == Button1)
                {
                    // Left click
                    mouseDownEvent->button = MouseButton::Left;
                }
                else if (event.xbutton.button == Button2)
                {
                    // Middle click
                    mouseDownEvent->button = MouseButton::Middle;
                }
                else if (event.xbutton.button == Button3)
                {
                    // Right click
                    mouseDownEvent->button = MouseButton::Right;
                }

                eventStack.push(mouseDownEvent);
                break;
            }
            case ButtonRelease:
            {
                MouseButtonUpEvent *mouseUpEvent = new MouseButtonUpEvent();

                if (event.xbutton.button == Button1)
                {
                    // Left click
                    mouseUpEvent->button = MouseButton::Left;
                }
                else if (event.xbutton.button == Button2)
                {
                    // Middle click
                    mouseUpEvent->button = MouseButton::Middle;
                }
                else if (event.xbutton.button == Button3)
                {
                    // Right click
                    mouseUpEvent->button = MouseButton::Right;
                }

                eventStack.push(mouseUpEvent);
                break;
            }

            case ClientMessage:
            {
                Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
                if (static_cast<Atom>(event.xclient.data.l[0]) == WM_DELETE_WINDOW)
                {
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

    bool Window::HasEvents()
    {
        return !eventStack.empty();
    }

    Event *Window::PopEvent()
    {
        Event *value = eventStack.top();
        eventStack.pop();

        return value;
    }

    void Nova::Window::LockCursor() {
        if (cursorLocked) return;

        // Create invisible cursor
        Pixmap bm_no;
        XColor black;
        static char no_data[] = { 0,0,0,0,0,0,0,0 };
        black.red = black.green = black.blue = 0;
        bm_no = XCreateBitmapFromData(display, window, no_data, 8, 8);
        invisibleCursor = XCreatePixmapCursor(display, bm_no, bm_no, &black, &black, 0, 0);
        XDefineCursor(display, window, invisibleCursor);
        XFreePixmap(display, bm_no);

        // Grab pointer
        XGrabPointer(display, window, True,
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                    GrabModeAsync, GrabModeAsync, window, None, CurrentTime);

        // Move to center
        int centerX = width / 2;
        int centerY = height / 2;
        XWarpPointer(display, None, window, 0, 0, 0, 0, centerX, centerY);
        XFlush(display);

        cursorLocked = true;
    }

    void Nova::Window::UnlockCursor() {
        if (!cursorLocked) return;

        XUngrabPointer(display, CurrentTime);
        XUndefineCursor(display, window);
        XFreeCursor(display, invisibleCursor);

        cursorLocked = false;
    }

}