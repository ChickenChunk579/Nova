#include <Nova/Nova.hpp>
#include <Rune/Rune.hpp>

int main() {
    Nova::Window window("Test", 1280, 720);

    Nova::X11PlatformData* platformData = (Nova::X11PlatformData*)window.platformData;

    Rune::Initialize(platformData->display, static_cast<uint32_t>(platformData->window), 1280, 720, false);

    int i = 0;
    while (window.PollEvents()) {
        Flux::Info("Hello: {}", i);
        i++;

        Rune::Clear(1, 0, 1, 1);

        Rune::PreSetupFrame();
        Rune::SetupFrame();

        Rune::FinishFrame();
    }
}