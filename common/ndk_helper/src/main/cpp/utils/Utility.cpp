#include "Utility.hpp"

glm::mat4 ndk_helper::utils::get_projection_matrix(const float width, const float height) {
    auto aspect = width / height;

    return glm::perspective(
            glm::radians(FOV_ANGLE),
            aspect,
            Z_NEAR,
            Z_FAR
    );
}

glm::vec3 ndk_helper::utils::get_light_dir() {
    return glm::vec3{0.0f, 1.0f, -2.0f};
}

glm::vec3 ndk_helper::utils::get_light_color() {
    return glm::vec3{0.5f, 0.5f, 0.5f};
}

float ndk_helper::utils::get_aspect_ratio(const float width, const float height) {
    return width / height;
}

template<typename T, typename ... Args>
std::unique_ptr<T> ndk_helper::utils::make_unique(Args&& ... args) {
    return std::unique_ptr<T>{new T{args...}};
}