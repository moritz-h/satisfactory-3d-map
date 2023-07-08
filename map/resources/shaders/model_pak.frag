#version 450

uniform int selectedId;

in vec3 position;
in vec4 tangent_x;
in vec4 tangent_z;
in vec2 texCoord;
flat in int id;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec4 color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    if (id == selectedId) {
        color = color * 0.5f + vec4(0.0f, 0.5f, 0.0f, 0.5f);
    }
    fragOutAlbedo = color;
    fragOutNormal = vec4(normalize(tangent_z.xyz), 0.0f);
    fragOutId = id;
}
