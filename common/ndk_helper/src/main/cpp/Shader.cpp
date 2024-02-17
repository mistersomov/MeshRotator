#include "Shader.hpp"

ndk_helper::shdr::Shader::Shader(GLuint id) noexcept : programId_{id} {}

ndk_helper::shdr::Shader::~Shader() {}

void ndk_helper::shdr::Shader::activate() const {
    glUseProgram(programId_);
}

GLuint ndk_helper::shdr::Shader::get_id() const {
    return programId_;
}

void ndk_helper::shdr::set_int(shdr::Shader* program, const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(program->get_id(), name.c_str()), value);
}

void ndk_helper::shdr::set_float(shdr::Shader* program, const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(program->get_id(), name.c_str()), value);
}

void ndk_helper::shdr::set_vec3(shdr::Shader* program, const std::string& name, const glm::vec3 value) {
    glUniform3fv(
        glGetUniformLocation(program->get_id(),name.c_str()),
        1,
        glm::value_ptr(value)
    );
}

void ndk_helper::shdr::set_mat4(shdr::Shader* program, const std::string& name, const glm::mat4 value) {
    glUniformMatrix4fv(
        glGetUniformLocation(program->get_id(),name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}