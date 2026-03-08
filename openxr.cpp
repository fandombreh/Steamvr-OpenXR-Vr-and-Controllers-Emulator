#include "openxr_runtime.h"
#include <iostream>
#include <cstring>

OpenXRRuntime::OpenXRRuntime()
    : hmd_(std::make_shared<HMD>()),
      left_controller_(std::make_shared<Controller>(Controller::Hand::LEFT)),
      right_controller_(std::make_shared<Controller>(Controller::Hand::RIGHT)) {
}

OpenXRRuntime::~OpenXRRuntime() {
    Shutdown();
}

bool OpenXRRuntime::Initialize() {
    if (!CreateInstance()) {
        std::cerr << "Failed to create XR instance" << std::endl;
        return false;
    }

    if (!CreateSystemAndSession()) {
        std::cerr << "Failed to create system and session" << std::endl;
        return false;
    }

    if (!SetupSpaces()) {
        std::cerr << "Failed to setup spaces" << std::endl;
        return false;
    }

    if (!SetupActions()) {
        std::cerr << "Failed to setup actions" << std::endl;
        return false;
    }

    return true;
}

bool OpenXRRuntime::CreateInstance() {
    XrInstanceCreateInfo instance_create_info{};
    instance_create_info.type = XR_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.next = nullptr;
    instance_create_info.createFlags = 0;
    instance_create_info.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
    strcpy(instance_create_info.applicationInfo.applicationName, "SteamVR OpenXR Emulator");
    instance_create_info.applicationInfo.applicationVersion = 1;
    strcpy(instance_create_info.applicationInfo.engineName, "Custom");
    instance_create_info.applicationInfo.engineVersion = 1;
    instance_create_info.enabledExtensionCount = 0;
    instance_create_info.enabledExtensionNames = nullptr;

    XrResult result = xrCreateInstance(&instance_create_info, &instance_);
    return result == XR_SUCCESS;
}

bool OpenXRRuntime::CreateSystemAndSession() {
    XrSystemGetInfo system_get_info{};
    system_get_info.type = XR_TYPE_SYSTEM_GET_INFO;
    system_get_info.next = nullptr;
    system_get_info.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    XrResult result = xrGetSystem(instance_, &system_get_info, &system_id_);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to get system: " << result << std::endl;
        return false;
    }

    XrSessionCreateInfo session_create_info{};
    session_create_info.type = XR_TYPE_SESSION_CREATE_INFO;
    session_create_info.next = nullptr;
    session_create_info.createFlags = 0;
    session_create_info.systemId = system_id_;

    result = xrCreateSession(instance_, &session_create_info, &session_);
    return result == XR_SUCCESS;
}

bool OpenXRRuntime::SetupSpaces() {
    XrReferenceSpaceCreateInfo reference_space_create_info{};
    reference_space_create_info.type = XR_TYPE_REFERENCE_SPACE_CREATE_INFO;
    reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
    reference_space_create_info.poseInReferenceSpace = {
        {0, 0, 0, 1},  // identity quaternion
        {0, 0, 0}      // zero position
    };

    XrResult result = xrCreateReferenceSpace(session_, &reference_space_create_info, &local_space_);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to create local space: " << result << std::endl;
        return false;
    }

    reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    result = xrCreateReferenceSpace(session_, &reference_space_create_info, &stage_space_);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to create stage space: " << result << std::endl;
        return false;
    }

    return true;
}

bool OpenXRRuntime::SetupActions() {
    XrActionSetCreateInfo action_set_create_info{};
    action_set_create_info.type = XR_TYPE_ACTION_SET_CREATE_INFO;
    action_set_create_info.next = nullptr;
    strcpy(action_set_create_info.actionSetName, "default_actions");
    strcpy(action_set_create_info.localizedActionSetName, "Default Actions");
    action_set_create_info.priority = 0;

    XrResult result = xrCreateActionSet(instance_, &action_set_create_info, &action_set_);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to create action set: " << result << std::endl;
        return false;
    }

    XrActionCreateInfo action_create_info{};
    action_create_info.type = XR_TYPE_ACTION_CREATE_INFO;
    action_create_info.next = nullptr;
    action_create_info.actionType = XR_ACTION_TYPE_POSE_INPUT;
    strcpy(action_create_info.actionName, "hand_pose");
    strcpy(action_create_info.localizedActionName, "Hand Pose");
    action_create_info.countSubactionPaths = 0;
    action_create_info.subactionPaths = nullptr;

    result = xrCreateAction(action_set_, &action_create_info, &pose_action_);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to create pose action: " << result << std::endl;
        return false;
    }

    return true;
}

bool OpenXRRuntime::CreateSession() {
    if (session_ == XR_NULL_HANDLE) {
        std::cerr << "Session not initialized" << std::endl;
        return false;
    }

    XrSessionBeginInfo session_begin_info{};
    session_begin_info.type = XR_TYPE_SESSION_BEGIN_INFO;
    session_begin_info.next = nullptr;
    session_begin_info.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

    XrResult result = xrBeginSession(session_, &session_begin_info);
    if (result == XR_SUCCESS) {
        session_running_ = true;
        return true;
    }

    std::cerr << "Failed to begin session: " << result << std::endl;
    return false;
}

bool OpenXRRuntime::CreateSwapchains() {
    // Implementation for swapchain creation
    // This would involve getting view configurations and creating rendering surfaces
    return true;
}

bool OpenXRRuntime::PollEvents() {
    XrEventDataBuffer event_data{};
    event_data.type = XR_TYPE_EVENT_DATA_BUFFER;
    event_data.next = nullptr;

    XrResult result = xrPollEvent(instance_, &event_data);
    while (result == XR_SUCCESS) {
        switch (event_data.type) {
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                auto* session_state_changed = reinterpret_cast<XrEventDataSessionStateChanged*>(&event_data);
                std::cout << "Session state changed to: " << session_state_changed->state << std::endl;
                break;
            }
            case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
                std::cout << "Instance loss pending" << std::endl;
                return false;
            }
            default:
                break;
        }

        event_data.type = XR_TYPE_EVENT_DATA_BUFFER;
        result = xrPollEvent(instance_, &event_data);
    }

    return true;
}

bool OpenXRRuntime::FrameLoop() {
    if (!session_running_) {
        return false;
    }

    XrFrameWaitInfo frame_wait_info{};
    frame_wait_info.type = XR_TYPE_FRAME_WAIT_INFO;
    frame_wait_info.next = nullptr;

    XrResult result = xrWaitFrame(session_, &frame_wait_info, &frame_state_);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to wait frame: " << result << std::endl;
        return false;
    }

    // Update controller and HMD states
    left_controller_->Update();
    right_controller_->Update();
    hmd_->Update();

    XrFrameBeginInfo frame_begin_info{};
    frame_begin_info.type = XR_TYPE_FRAME_BEGIN_INFO;
    frame_begin_info.next = nullptr;

    result = xrBeginFrame(session_, &frame_begin_info);
    if (result != XR_SUCCESS) {
        std::cerr << "Failed to begin frame: " << result << std::endl;
        return false;
    }

    XrFrameEndInfo frame_end_info{};
    frame_end_info.type = XR_TYPE_FRAME_END_INFO;
    frame_end_info.next = nullptr;
    frame_end_info.displayTime = frame_state_.predictedDisplayTime;
    frame_end_info.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
    frame_end_info.layerCount = 0;
    frame_end_info.layers = nullptr;

    result = xrEndFrame(session_, &frame_end_info);
    return result == XR_SUCCESS;
}

void OpenXRRuntime::Shutdown() {
    if (local_space_ != XR_NULL_HANDLE) {
        xrDestroySpace(local_space_);
        local_space_ = XR_NULL_HANDLE;
    }

    if (stage_space_ != XR_NULL_HANDLE) {
        xrDestroySpace(stage_space_);
        stage_space_ = XR_NULL_HANDLE;
    }

    if (action_set_ != XR_NULL_HANDLE) {
        xrDestroyActionSet(action_set_);
        action_set_ = XR_NULL_HANDLE;
    }

    if (session_ != XR_NULL_HANDLE) {
        xrEndSession(session_);
        xrDestroySession(session_);
        session_ = XR_NULL_HANDLE;
    }

    if (instance_ != XR_NULL_HANDLE) {
        xrDestroyInstance(instance_);
        instance_ = XR_NULL_HANDLE;
    }
}
