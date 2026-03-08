#pragma once

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <vector>
#include <memory>
#include "hmd.h"
#include "controller.h"

class OpenXRRuntime {
public:
    OpenXRRuntime();
    ~OpenXRRuntime();

    bool Initialize();
    bool CreateSession();
    bool CreateSwapchains();
    bool PollEvents();
    bool FrameLoop();
    void Shutdown();

    XrInstance GetInstance() const { return instance_; }
    XrSession GetSession() const { return session_; }
    XrSpace GetLocalSpace() const { return local_space_; }

    std::shared_ptr<HMD> GetHMD() { return hmd_; }
    std::shared_ptr<Controller> GetLeftController() { return left_controller_; }
    std::shared_ptr<Controller> GetRightController() { return right_controller_; }

private:
    bool CreateInstance();
    bool CreateSystemAndSession();
    bool SetupSpaces();
    bool SetupActions();

    XrInstance instance_ = XR_NULL_HANDLE;
    XrSession session_ = XR_NULL_HANDLE;
    XrSystemId system_id_ = XR_NULL_SYSTEM_ID;
    XrSpace local_space_ = XR_NULL_HANDLE;
    XrSpace stage_space_ = XR_NULL_HANDLE;
    XrSpace view_space_ = XR_NULL_HANDLE;

    // Action sets for input
    XrActionSet action_set_ = XR_NULL_HANDLE;
    XrAction pose_action_ = XR_NULL_HANDLE;
    XrAction trigger_action_ = XR_NULL_HANDLE;
    XrAction grip_action_ = XR_NULL_HANDLE;

    std::shared_ptr<HMD> hmd_;
    std::shared_ptr<Controller> left_controller_;
    std::shared_ptr<Controller> right_controller_;

    bool session_running_ = false;
    XrFrameState frame_state_;
};
