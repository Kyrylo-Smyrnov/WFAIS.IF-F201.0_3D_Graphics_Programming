#version 420

uniform sampler2D map_Kd;

layout(std140, binding = 0) uniform KdMaterial {
    vec4 Kd;
    int use_vertex_color;
    int use_map_Kd;
};

in vec3 vertex_color;
in vec2 tex_coord;

layout(location=0) out vec4 vFragColor;

vec3 srgb_gamma_correction(vec3 color) {
   color = clamp(color, 0.0, 1.0);
   color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
   return color;
}

void main() {
    vec4 texture_color = texture(map_Kd, tex_coord);

    vFragColor = Kd;
    if(use_vertex_color != 0)
    {
        vFragColor.a = (vFragColor * vec4(vertex_color, 1.0)).a;
        vFragColor.rgb = srgb_gamma_correction(vFragColor.rgb * vec4(vertex_color, 1.0).rgb).rgb;
    }
    if(use_map_Kd != 0)
    {
        vFragColor.a = texture_color.a;
        vFragColor.rgb = srgb_gamma_correction(texture_color.rgb);
    }
}
