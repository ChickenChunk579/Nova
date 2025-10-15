#pragma once
#include <wayland-client.h>
#include <wayland-egl.h>
#include <Flux/Flux.hpp>
#include <string>
#define Window X11Window
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#undef Window

namespace Nova {
    class PlatformData {
    public:
        virtual ~PlatformData() = default;
    };
    
    class X11PlatformData : public PlatformData {
    public:
        Display* display;
        X11Window window;
    };

    enum class Backend {
        Wayland,
        X11
    };

    class Window {
    private:
        Display* display;
        X11Window window;
        int screen;
    public:
        PlatformData* platformData;
        Backend backend;

        Window(std::string title, int width, int height);

        bool PollEvents();
    };
}