#include "Utility.hpp"

glm::mat4 ndk_helper::utils::get_projection_matrix(const float width, const float height) {
    return glm::perspective(
        FOV_ANGLE,
        get_aspect_ratio(width, height),
        Z_NEAR,
        Z_FAR
    );
}

glm::mat4 ndk_helper::utils::get_view_matrix(glm::vec3 viewPos) {
    return glm::lookAt(
        viewPos,
        glm::vec3(0.0f),
        glm::vec3{0.0f, 1.0f, 0.0f}
    );
}

glm::vec3 ndk_helper::utils::get_light_dir() {
    return glm::vec3{-10.0f, 10.0f, -10.0f};
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