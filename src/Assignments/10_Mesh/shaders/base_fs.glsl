#version 410

layout(location=0) out vec4 vFragColor;

in vec3 vertex_color;

void main() {
    vFragColor = vec4(vertex_color, 1.0);
}
