#include "openxr_runtime.h"
#include "input_handler.h"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    std::cout << "Starting SteamVR OpenXR Emulator..." << std::endl;

    // Create and initialize the OpenXR runtime
    OpenXRRuntime runtime;

    if (!runtime.Initialize()) {
        std::cerr << "Failed to initialize OpenXR runtime" << std::endl;
        return 1;
    }

    std::cout << "OpenXR runtime initialized successfully" << std::endl;

    // Create session
    if (!runtime.CreateSession()) {
        std::cerr << "Failed to create OpenXR session" << std::endl;
        return 1;
    }

    std::cout << "OpenXR session created successfully" << std::endl;

    // Setup input handler
    InputHandler input_handler(
        runtime.GetHMD(),
        runtime.GetLeftController(),
        runtime.GetRightController()
    );

    // Main loop
    bool running = true;
    auto start_time = std::chrono::high_resolution_clock::now();
    uint32_t frame_count = 0;

    while (running) {
        auto frame_start = std::chrono::high_resolution_clock::now();

        // Poll events
        if (!runtime.PollEvents()) {
            std::cout << "Runtime requested shutdown" << std::endl;
            running = false;
            break;
        }

        // Update input
        input_handler.ProcessInput();

        // Simulate movements
        auto elapsed = std::chrono::duration<float>(
            std::chrono::high_resolution_clock::now() - start_time
        ).count();
        
        input_handler.SimulateHeadMovement(elapsed);
        input_handler.SimulateControllerMovement(elapsed);

        // Frame loop
        if (!runtime.FrameLoop()) {
            std::cerr << "Frame loop failed" << std::endl;
            running = false;
            break;
        }

        frame_count++;

        // Print stats every 300 frames (~10 seconds at 30fps)
        if (frame_count % 300 == 0) {
            std::cout << "Frame: " << frame_count 
                      << " | HMD Pos: (" 
                      << runtime.GetHMD()->GetState().position.x << ", "
                      << runtime.GetHMD()->GetState().position.y << ", "
                      << runtime.GetHMD()->GetState().position.z << ")" << std::endl;
        }

        // Cap at ~90 FPS (typical VR refresh rate)
        auto frame_duration = std::chrono::high_resolution_clock::now() - frame_start;
        auto target_duration = std::chrono::milliseconds(11);  // ~90 FPS
        if (frame_duration < target_duration) {
            std::this_thread::sleep_for(target_duration - frame_duration);
        }
    }

    std::cout << "Shutting down..." << std::endl;
    runtime.Shutdown();

    std::cout << "Total frames rendered: " << frame_count << std::endl;
    std::cout << "Emulator shutdown complete" << std::endl;

    return 0;
}
