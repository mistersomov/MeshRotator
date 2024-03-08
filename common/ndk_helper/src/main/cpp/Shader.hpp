#ifndef MESH_ROTATOR_SHADER_HPP
#define MESH_ROTATOR_SHADER_HPP

#include <GLES3/gl32.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>

namespace ndk_helper {
    namespace shdr {
        struct Shader {
            Shader(GLuint) noexcept;

            void activate() const;
            void set_int(const std::string& name, int value) const;
            void set_float(const std::string& name, float value) const;
            void set_vec3(const std::string& name, const glm::vec3 value) const;
            void set_mat4(const std::string& name, const glm::mat4 value) const;

            GLuint id_;
        };
    }
}

#endif //MESH_ROTATOR_SHADER_HPP