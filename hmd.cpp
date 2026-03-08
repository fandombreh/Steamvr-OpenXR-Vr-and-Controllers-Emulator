#include "hmd.h"
#include <glm/gtc/matrix_transform.hpp>

HMD::HMD() {
    // Initialize eye positions based on IPD
    UpdateEyeViews();
}

HMD::~HMD() = default;

void HMD::Update() {
    previous_state_ = state_;
    state_.velocity = (state_.position - previous_state_.position);
    UpdateEyeViews();
}

void HMD::SetState(const State& state) {
    previous_state_ = state_;
    state_ = state;
    UpdateEyeViews();
}

void HMD::SetPosition(const glm::vec3& position) {
    state_.position = position;
    UpdateEyeViews();
}

void HMD::SetOrientation(const glm::quat& orientation) {
    state_.orientation = orientation;
    UpdateEyeViews();
}

void HMD::SetIPD(float ipd) {
    state_.ipd = ipd;
    UpdateEyeViews();
}

void HMD::UpdateEyeViews() {
    float half_ipd = state_.ipd / 2.0f;

    // Left eye offset
    state_.left_eye.position = state_.position + glm::vec3(-half_ipd, 0.0f, 0.0f);
    state_.left_eye.orientation = state_.orientation;

    // Right eye offset
    state_.right_eye.position = state_.position + glm::vec3(half_ipd, 0.0f, 0.0f);
    state_.right_eye.orientation = state_.orientation;
}
