#include "Utility.hpp"

glm::vec3 ndk_helper::utils::get_light_dir() {
    return glm::vec3{-10.0f, 10.0f, -10.0f};
}

glm::vec3 ndk_helper::utils::get_light_color() {
    return glm::vec3{0.5f, 0.5f, 0.5f};
}