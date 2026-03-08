#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>

namespace MathUtils {
    // Convert Euler angles to quaternion
    inline glm::quat EulerToQuat(float yaw, float pitch, float roll) {
        glm::quat q_yaw = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat q_pitch = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat q_roll = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
        return q_yaw * q_pitch * q_roll;
    }

    // Convert quaternion to Euler angles
    inline glm::vec3 QuatToEuler(const glm::quat& q) {
        glm::vec3 euler;

        float test = 2.0f * (q.w * q.y - q.z * q.x);
        if (test > 0.99999f) {
            euler.y = 2.0f * std::atan2(q.x, q.w);
            euler.z = 0.0f;
            euler.x = 0.0f;
            return euler;
        }
        if (test < -0.99999f) {
            euler.y = -2.0f * std::atan2(q.x, q.w);
            euler.z = 0.0f;
            euler.x = 0.0f;
            return euler;
        }

        euler.x = std::atan2(2.0f * (q.w * q.x + q.y * q.z), 1.0f - 2.0f * (q.x * q.x + q.y * q.y));
        euler.y = std::asin(test);
        euler.z = std::atan2(2.0f * (q.w * q.z + q.x * q.y), 1.0f - 2.0f * (q.y * q.y + q.z * q.z));

        return euler;
    }

    // Normalize a vector
    inline glm::vec3 Normalize(const glm::vec3& v) {
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (length == 0.0f) return glm::vec3(0.0f);
        return glm::vec3(v.x / length, v.y / length, v.z / length);
    }
}
