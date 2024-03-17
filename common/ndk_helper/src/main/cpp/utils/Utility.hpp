#ifndef MESHROTATOR_UTILITY_HPP
#define MESHROTATOR_UTILITY_HPP

#include "Constants.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <memory>

namespace ndk_helper {
    namespace utils {
        glm::mat4 get_projection_matrix(const float width, const float height);
        glm::mat4 get_view_matrix(glm::vec3 viewPos);
        float get_aspect_ratio(const float width, const float height);

        glm::vec3 get_light_dir();
        glm::vec3 get_light_color();

        template<typename T, typename ... Args>
        std::unique_ptr<T> make_unique(Args&& ... args);
    }
}

#endif //MESHROTATOR_UTILITY_HPP
