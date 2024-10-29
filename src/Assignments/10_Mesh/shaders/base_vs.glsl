#version 420

layout(std140, binding = 0) uniform Transformations {
    mat4 PVM;
};

layout(location=0) in vec4 a_vertex_position;
layout(location=5) in vec3 a_vertex_color;

out vec3 vertex_color;

void main() {
    gl_Position = PVM * a_vertex_position;
    vertex_color = a_vertex_color;
}
