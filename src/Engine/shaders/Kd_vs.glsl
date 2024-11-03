#version 420

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

layout(location=0) in vec4 a_vertex_position;
layout(location=3) in vec2 a_texture_coord;
layout(location=5) in vec3 a_vertex_color;

out vec3 vertex_color;
out vec2 tex_coord;

void main() {
    gl_Position = PVM * a_vertex_position;
    
    vertex_color = a_vertex_color;
    tex_coord = a_texture_coord;
}
