#version 420

layout(std140, binding = 0) uniform Mixer {
    float strength;
    vec3 mix_color;
};

layout(location=0) out vec4 vFragColor;

in vec3 vertex_color;

void main() {
    vFragColor = vec4((vertex_color + mix_color) * strength, 1.0);
}
