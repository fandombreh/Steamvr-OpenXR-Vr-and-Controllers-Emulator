# SteamVR OpenXR Emulator

A C++ emulator for Steam VR using the OpenXR standard. This project provides a complete OpenXR runtime implementation with HMD and controller support for development and testing purposes.

## Features

- **OpenXR Runtime**: Full OpenXR instance and session management
- **HMD Emulation**: Head-mounted display simulation with position and orientation tracking
- **Controller Support**: Left and right controller emulation with:
  - Position and orientation tracking
  - Button input simulation (trigger, grip, menu, A/B/X/Y)
  - Touchpad/analog stick input
  - Analog trigger and grip values
- **Input Handler**: Simulated head and controller movement
- **Action System**: OpenXR action sets and actions for input binding

## Requirements

- C++17 or later
- CMake 3.20+
- OpenXR SDK
- GLM (Header-only)
- pthread library

## Building

### Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
```

### Windows (MSVC)

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

## Dependencies

Install dependencies:

### Ubuntu/Debian
```bash
sudo apt-get install cmake pkg-config
```

### macOS
```bash
brew install cmake glm
```

Download OpenXR SDK from: https://github.com/KhronosGroup/OpenXR-SDK

## Usage

```bash
./steamvr-emulator
```

The emulator will:
1. Initialize the OpenXR runtime
2. Create an OpenXR session
3. Simulate HMD and controller movements
4. Display tracking information to console

Press Ctrl+C to exit.

## Architecture

### Core Components

- **OpenXRRuntime**: Main runtime manager handling instance, session, and space management
- **HMD**: Head-mounted display state tracking with eye position/orientation
- **Controller**: VR controller simulation with buttons, triggers, and positioning
- **InputHandler**: Input processing and simulation

### Data Flow

```
InputHandler
    ↓
Controller/HMD State Updates
    ↓
OpenXR Runtime
    ↓
xrWaitFrame → xrBeginFrame → xrEndFrame
```

## Extending the Emulator

### Adding Real Input Support

Implement keyboard/mouse/gamepad input in `InputHandler::ProcessInput()`:

```cpp
void InputHandler::ProcessInput() {
    // Check keyboard for head movement
    // Check mouse for controller rotation
    // Check gamepad for triggers/buttons
}
```

### Adding Graphics Backend

Integrate rendering with Vulkan or OpenGL:

```cpp
// In main loop after xrBeginFrame:
renderer->RenderFrame(
    runtime.GetHMD()->GetState(),
    runtime.GetLeftController()->GetState(),
    runtime.GetRightController()->GetState()
);
```

### Adding Force Feedback

Extend Controller class with haptic feedback:

```cpp
class Controller {
    void SetVibration(float frequency, float amplitude);
};
```

## Troubleshooting

### OpenXR Runtime Not Found

Ensure OpenXR SDK is properly installed and CMake can find it:

```bash
cmake .. -DOPENXR_DIR=/path/to/openxr/sdk
```

### Compilation Errors

Verify C++17 support:

```bash
cmake .. -DCMAKE_CXX_STANDARD=17
```

## License

MIT License - See LICENSE file for details

## References

- OpenXR Specification: https://www.khronos.org/openxr/
- OpenXR SDK: https://github.com/KhronosGroup/OpenXR-SDK
- GLM Documentation: https://glm.g-truc.net/

## Future Enhancements

- [ ] Hand pose tracking
- [ ] Eye gaze tracking
- [ ] Advanced haptic feedback
- [ ] Multiple camera support
- [ ] Performance profiling tools
- [ ] Network streaming for remote access
- [ ] Integration with popular game engines
- [ ] Motion capture integration
