cmake_minimum_required(VERSION 3.20)
project(SteamVROpenXREmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenXR
find_package(OpenXR REQUIRED)

# Add executable
add_executable(steamvr-emulator
    src/main.cpp
    src/openxr_runtime.cpp
    src/controller.cpp
    src/hmd.cpp
    src/input_handler.cpp
)

# Link libraries
target_link_libraries(steamvr-emulator
    PRIVATE
    OpenXR::openxr_loader
    pthread
)

# Include directories
target_include_directories(steamvr-emulator
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)
