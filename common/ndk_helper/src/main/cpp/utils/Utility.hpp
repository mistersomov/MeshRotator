#ifndef MESHROTATOR_UTILITY_HPP
#define MESHROTATOR_UTILITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <memory>

namespace ndk_helper {
    namespace utils {
        glm::vec3 get_light_dir();
        glm::vec3 get_light_color();

        template<typename T, typename ... Args>
        std::unique_ptr<T> make_unique(Args&& ... args) {
            return std::unique_ptr<T>{new T{args...}};
        };
    }
}

#endif //MESHROTATOR_UTILITY_HPP
