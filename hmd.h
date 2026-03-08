#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class HMD {
public:
    struct View {
        glm::vec3 position{0.0f};
        glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
    };

    struct State {
        glm::vec3 position{0.0f, 1.7f, 0.0f};  // Eye height
        glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 velocity{0.0f};
        glm::vec3 angular_velocity{0.0f};
        View left_eye;
        View right_eye;
        float ipd = 0.064f;  // Interpupillary distance (64mm)
        bool is_active = true;
    };

    HMD();
    ~HMD();

    void Update();
    void SetState(const State& state);
    const State& GetState() const { return state_; }

    void SetPosition(const glm::vec3& position);
    void SetOrientation(const glm::quat& orientation);
    void SetIPD(float ipd);

    bool IsActive() const { return state_.is_active; }
    void SetActive(bool active) { state_.is_active = active; }

private:
    void UpdateEyeViews();

    State state_;
    State previous_state_;
};
