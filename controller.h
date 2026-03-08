#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdint>

class Controller {
public:
    enum class Hand {
        LEFT = 0,
        RIGHT = 1
    };

    struct ButtonState {
        bool trigger = false;
        bool grip = false;
        bool menu = false;
        bool a_button = false;
        bool b_button = false;
        bool x_button = false;
        bool y_button = false;
    };

    struct TouchpadState {
        float x = 0.0f;
        float y = 0.0f;
        bool touched = false;
    };

    struct State {
        glm::vec3 position{0.0f};
        glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 velocity{0.0f};
        glm::vec3 angular_velocity{0.0f};
        ButtonState buttons;
        TouchpadState touchpad;
        float trigger_value = 0.0f;
        float grip_value = 0.0f;
        bool is_active = true;
    };

    Controller(Hand hand);
    ~Controller();

    void Update();
    void SetState(const State& state);
    const State& GetState() const { return state_; }

    void SetButtonPressed(uint32_t button_id, bool pressed);
    void SetTriggerValue(float value);
    void SetGripValue(float value);
    void SetTouchpadState(float x, float y, bool touched);

    Hand GetHand() const { return hand_; }
    bool IsActive() const { return state_.is_active; }
    void SetActive(bool active) { state_.is_active = active; }

private:
    Hand hand_;
    State state_;
    State previous_state_;
};
