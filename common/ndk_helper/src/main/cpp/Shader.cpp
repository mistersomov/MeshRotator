#include "Shader.hpp"

ndk_helper::shdr::Shader::Shader(GLuint id) noexcept : id_{id} {}

void ndk_helper::shdr::Shader::activate() const {
    glUseProgram(id_);
}

void ndk_helper::shdr::Shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void ndk_helper::shdr::Shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void ndk_helper::shdr::Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(
        glGetUniformLocation(id_, name.c_str()),
        static_cast<uint32_t>(value)
    );
}

void ndk_helper::shdr::Shader::set_vec3(const std::string& name, const glm::vec3 value) const {
    glUniform3fv(
        glGetUniformLocation(id_, name.c_str()),
        1,
        glm::value_ptr(value)
    );
}

void ndk_helper::shdr::Shader::set_mat4(const std::string& name, const glm::mat4 value) const {
    glUniformMatrix4fv(
        glGetUniformLocation(id_, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}