#include "texture.h"
#include "stb/stb_image.h"
#include "Application/utils.h"
#include <iostream>

namespace xe {
    GLuint create_texture(const std::string& name, bool is_sRGB) {
        stbi_set_flip_vertically_on_load(true);
        GLint width, height, channels;
        auto texture_file = std::string(ROOT_DIR) + "/" + name;
        auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
        
        if (!img) {
            std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
        } else {
            std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
        }

        GLint format = is_sRGB ? GL_SRGB : GL_RGB;

        GLuint tex_handle;
        OGL_CALL(glGenTextures(1, &tex_handle));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, tex_handle));
        OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));

        return tex_handle;
    }
}