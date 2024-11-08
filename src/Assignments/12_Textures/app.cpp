//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "glm/gtc/matrix_transform.hpp"

#include "stb/stb_image.h"

#include "Application/utils.h"

#include "Engine/KdMaterial.h"

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    camera()->set_aspect(static_cast<float>(w) / static_cast<float>(h));
    OGL_CALL(glViewport(0, 0, w, h));
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 20.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if(controller_) {
        double x;
        double y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            controller_->LMB_pressed(x, y);
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            controller_->LMB_released(x, y);
        } 
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if(controller_) {
        controller_->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::init() {
    /*
     * A utility function that reads the shaders' source files, compiles them and creates the program object,
     * as everything in OpenGL we reference the program by an integer "handle".
     * The input to this function is a map that associates a shader type (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER) with
     * its source file. The source files are located in the PROJECT_DIR/shaders directory, where  PROJECT_DIR is the
     * current assignment directory (e.g., src/Assignments/Triangle).
     */

    std::vector<GLfloat> vertices = {
        -0.5f, 0.5f, 0.0f,  0.1910f, 0.5f,
        -0.5f, -0.5f, 0.0f, 0.5f, 0.1910f,
        0.5f, -0.5f, 0.0f,  0.8090f, 0.5f,
        0.5f, 0.5f, 0.0f,   0.5f, 0.8090f,

        0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,   0.0f, 1.0f
    };

    std::vector<GLubyte> indices = {
        0, 2, 1,
        0, 3, 2,
        0, 1, 4,
        1, 2, 5,
        2, 3, 6,
        3, 0, 7
    };

    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    
    if (!img) {
        std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
        std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }

    GLuint tex_handle;
    OGL_CALL(glGenTextures(1, &tex_handle));
    OGL_CALL(glBindTexture(GL_TEXTURE_2D, tex_handle));
    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));

    xe::KdMaterial::init();
    auto kd_white_material = new xe::KdMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    auto pyramid = new xe::Mesh(5 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW, indices.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(float), vertices.data());
    pyramid->load_indices(0, indices.size() * sizeof(GLubyte), indices.data());
    pyramid->add_attribute(xe::AttributeType::POSITION, 3, GL_FLOAT, 0 * sizeof(GLfloat));
    pyramid->add_attribute(xe::AttributeType::TEXCOORD_0, 2, GL_FLOAT, 3 * sizeof(GLfloat));

    auto kd_base_material = new xe::KdMaterial(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    auto kd_green_material = new xe::KdMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    auto kd_yellow_material = new xe::KdMaterial(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    auto kd_red_material = new xe::KdMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    auto kd_blue_material = new xe::KdMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    pyramid->add_primitive(0, 18, new xe::KdMaterial({1.0f, 1.0f, 1.0f, 1.0f}, tex_handle));

    add_mesh(pyramid);

    set_camera(new xe::Camera);
    set_controller(new xe::CameraController(camera()));

    camera()->perspective(glm::radians(45.0f),
                        static_cast<float>(frame_buffer_size().first) / static_cast<float>(frame_buffer_size().second),
                        0.1f, 20.0f);
    camera()->look_at(glm::vec3(2.0f, 1.0f, 2.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f));

    M_ = glm::mat4(1.0f);

    /*
     * All the calls to the OpenGL API are "encapsulated" in the OGL_CALL macro for debugging purposes as explained in
     * Assignments/DEBUGGING.md. The macro is defined in src/Application/utils.h. If the call to the OpenGL API returns an
     * error code, the macro will print the name of the function that failed, the file and line number where the error
     * occurred.
     */

    OGL_CALL(glCreateBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glNamedBufferData(u_trans_buffer_handle_, 16 * sizeof(float), NULL, GL_STATIC_DRAW));

    // Setting the background color of the rendering window,
    // I suggest not using white or black for better debugging.
    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    // This set up an OpenGL viewport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glEnable(GL_CULL_FACE));
    OGL_CALL(glEnable(GL_DEPTH_TEST));
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    glm::mat4 PVM = camera()->projection() * camera()->view() * M_;

    OGL_CALL(glNamedBufferSubData(u_trans_buffer_handle_, 0 * sizeof(float), 16 * sizeof(float), &PVM[0]));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));

    for(auto m : meshes_) {
        m->draw();
    }
}