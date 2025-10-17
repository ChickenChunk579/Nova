#include <Nova/Nova.hpp>
#include <Rune/Rune.hpp>

int main() {
    Flux::Info("Creating window...");
    Nova::Window window("Test", 1280, 720);
    
    Flux::Info("Getting platform data...");
    Nova::X11PlatformData* platformData = (Nova::X11PlatformData*)window.platformData;
    
    Flux::Info("Initializing Rune...");
    Rune::Initialize(platformData->display, static_cast<uint32_t>(platformData->window), 1280, 720, false);
    
    Flux::Info("Start main loop");
    int i = 0;
    
    // Check if PollEvents works at all
    bool shouldContinue = window.PollEvents();
    Flux::Info("First PollEvents returned: {}", shouldContinue);
    
    while (window.PollEvents()) {
        while (window.HasEvents())
        {
            window.PopEvent();
        }
        Flux::Info("Frame: {}", i);
        i++;
        
        Rune::Clear(1, 0, 1, 1);  // Magenta clear color
        Rune::PreSetupFrame();
        Rune::SetupFrame();
        Rune::FinishFrame();
    }
    
    Flux::Info("Loop exited after {} iterations", i);
    return 0;
}