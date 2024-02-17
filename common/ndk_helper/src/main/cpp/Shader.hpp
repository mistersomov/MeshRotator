#ifndef MESH_ROTATOR_SHADER_HPP
#define MESH_ROTATOR_SHADER_HPP

#include <GLES3/gl32.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>

namespace ndk_helper {
    namespace shdr {
        class Shader {
            GLuint programId_;
            std::unordered_map<std::string, GLuint> uniforms_;
        public:
            Shader(GLuint) noexcept;
            ~Shader();

            void activate() const;
            GLuint get_id() const;
        };

        void set_int(Shader*, const std::string&, int);
        void set_float(Shader*, const std::string&, float);
        void set_vec3(Shader*, const std::string&, const glm::vec3);
        void set_mat4(Shader*, const std::string&, const glm::mat4);
    }
}

#endif //MESH_ROTATOR_SHADER_HPP