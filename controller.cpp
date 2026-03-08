#include "controller.h"
#include <glm/gtc/matrix_transform.hpp>

Controller::Controller(Hand hand)
    : hand_(hand) {
    // Initialize default positions
    if (hand == Hand::LEFT) {
        state_.position = glm::vec3(-0.2f, -0.1f, -0.3f);
    } else {
        state_.position = glm::vec3(0.2f, -0.1f, -0.3f);
    }
}

Controller::~Controller() = default;

void Controller::Update() {
    previous_state_ = state_;
    // Update velocity based on position change
    state_.velocity = (state_.position - previous_state_.position);
}

void Controller::SetState(const State& state) {
    previous_state_ = state_;
    state_ = state;
}

void Controller::SetButtonPressed(uint32_t button_id, bool pressed) {
    switch (button_id) {
        case 0:
            state_.buttons.trigger = pressed;
            break;
        case 1:
            state_.buttons.grip = pressed;
            break;
        case 2:
            state_.buttons.menu = pressed;
            break;
        case 3:
            state_.buttons.a_button = pressed;
            break;
        case 4:
            state_.buttons.b_button = pressed;
            break;
        case 5:
            state_.buttons.x_button = pressed;
            break;
        case 6:
            state_.buttons.y_button = pressed;
            break;
        default:
            break;
    }
}

void Controller::SetTriggerValue(float value) {
    state_.trigger_value = glm::clamp(value, 0.0f, 1.0f);
    if (state_.trigger_value > 0.5f) {
        state_.buttons.trigger = true;
    } else {
        state_.buttons.trigger = false;
    }
}

void Controller::SetGripValue(float value) {
    state_.grip_value = glm::clamp(value, 0.0f, 1.0f);
    if (state_.grip_value > 0.5f) {
        state_.buttons.grip = true;
    } else {
        state_.buttons.grip = false;
    }
}

void Controller::SetTouchpadState(float x, float y, bool touched) {
    state_.touchpad.x = glm::clamp(x, -1.0f, 1.0f);
    state_.touchpad.y = glm::clamp(y, -1.0f, 1.0f);
    state_.touchpad.touched = touched;
}
