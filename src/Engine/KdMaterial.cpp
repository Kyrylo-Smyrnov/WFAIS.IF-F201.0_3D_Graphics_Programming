#include "KdMaterial.h"

xe::KdMaterial::KdMaterial(const glm::vec4 &Kd) : Kd_(Kd), use_vertex_colors_(0), use_Kd_map_(0), texture_(0) {}

xe::KdMaterial::KdMaterial(const glm::vec4 &Kd, const GLuint texture) : Kd_(Kd), texture_(texture) {}

GLint xe::KdMaterial::map_Kd_location = -1;

void xe::KdMaterial::init() {
    create_material_uniform_buffer(2 * sizeof(glm::vec4));
    create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                                {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});

    map_Kd_location = OGL_CALL(glGetUniformLocation(program(),"map_Kd"));
    if (map_Kd_location == -1) {
        SPDLOG_WARN("Cannot find map_Kd uniform");
    }
}

void xe::KdMaterial::bind() const {
    glUseProgram(program());

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer()));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));

    if (texture_ > 0) {
        int use_map_Kd = 1;
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), &use_map_Kd));
        OGL_CALL(glActiveTexture(GL_TEXTURE0));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
        OGL_CALL(glUniform1i(map_Kd_location, 0));
    } else {
        int use_map_Kd = 0;
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), &use_map_Kd));
    }
}

void xe::KdMaterial::unbind() const {
    if (texture_ > 0) {
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    }
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , 0));
}