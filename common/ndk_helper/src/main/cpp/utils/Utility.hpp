#ifndef MESHROTATOR_UTILITY_HPP
#define MESHROTATOR_UTILITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <memory>

namespace ndk_helper {
    namespace utils {
        constexpr float FOV_ANGLE = 45.0f;
        constexpr float Z_NEAR = 0.1f;
        constexpr float Z_FAR = 50.0f;

        glm::mat4 get_projection_matrix(const float width, const float height);
        float get_aspect_ratio(const float width, const float height);

        glm::vec3 get_light_dir();
        glm::vec3 get_light_color();

        template<typename T, typename ... Args>
        std::unique_ptr<T> make_unique(Args&& ... args);
    }
}

#endif //MESHROTATOR_UTILITY_HPP
