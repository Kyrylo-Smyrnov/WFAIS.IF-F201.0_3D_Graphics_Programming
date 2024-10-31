#version 420

layout(std140, binding = 0) uniform KdMaterial {
    vec4 Kd;
    int use_vertex_color;
};

layout(location=0) out vec4 vFragColor;

in vec3 vertex_color;

void main() {
    vFragColor = Kd;
    if(use_vertex_color != 0)
    {
        vFragColor = vFragColor * vec4(vertex_color, 1.0);
    }
}
