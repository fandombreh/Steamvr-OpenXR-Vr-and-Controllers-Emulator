#pragma once

#include "controller.h"
#include "hmd.h"
#include <memory>

class InputHandler {
public:
    InputHandler(std::shared_ptr<HMD> hmd,
                 std::shared_ptr<Controller> left_controller,
                 std::shared_ptr<Controller> right_controller);

    void ProcessInput();
    void SimulateHeadMovement(float delta_time);
    void SimulateControllerMovement(float delta_time);

private:
    std::shared_ptr<HMD> hmd_;
    std::shared_ptr<Controller> left_controller_;
    std::shared_ptr<Controller> right_controller_;
    
    float head_yaw_ = 0.0f;
    float head_pitch_ = 0.0f;
    float head_roll_ = 0.0f;
};
