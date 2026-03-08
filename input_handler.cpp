#include "input_handler.h"
#include <glm/gtc/quaternion.hpp>
#include <cmath>

InputHandler::InputHandler(std::shared_ptr<HMD> hmd,
                           std::shared_ptr<Controller> left_controller,
                           std::shared_ptr<Controller> right_controller)
    : hmd_(hmd),
      left_controller_(left_controller),
      right_controller_(right_controller) {
}

void InputHandler::ProcessInput() {
    // Simulate controller button inputs and positions
    // This would integrate with actual input devices (keyboard, mouse, gamepad)
}

void InputHandler::SimulateHeadMovement(float delta_time) {
    // Simple head rotation simulation
    head_yaw_ += delta_time * 0.5f;
    head_pitch_ += delta_time * 0.2f;

    glm::quat yaw = glm::angleAxis(head_yaw_, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitch = glm::angleAxis(head_pitch_, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat roll = glm::angleAxis(head_roll_, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::quat orientation = yaw * pitch * roll;
    hmd_->SetOrientation(orientation);
}

void InputHandler::SimulateControllerMovement(float delta_time) {
    // Simulate circular controller movement
    float time = delta_time;
    
    // Left controller in circular motion
    float left_x = std::sin(time) * 0.3f - 0.2f;
    float left_y = -0.1f + std::cos(time * 0.5f) * 0.1f;
    float left_z = -0.3f + std::cos(time) * 0.2f;
    left_controller_->SetState({
        glm::vec3(left_x, left_y, left_z),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
    });

    // Right controller mirrored
    float right_x = std::sin(time + 3.14f) * 0.3f + 0.2f;
    float right_y = -0.1f + std::cos(time * 0.5f) * 0.1f;
    float right_z = -0.3f + std::cos(time + 3.14f) * 0.2f;
    right_controller_->SetState({
        glm::vec3(right_x, right_y, right_z),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f)
    });
}
