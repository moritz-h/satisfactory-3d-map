#version 450

uniform sampler2D tex;
uniform int selectedId;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;
flat in int id;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec4 color = texture(tex, tex_coord);
    if (id == selectedId) {
        color = color * 0.5f + vec4(0.0f, 0.5f, 0.0f, 0.5f);
    }
    fragOutAlbedo = color;
    fragOutNormal = vec4(normalize(normal), 0.0f);
    fragOutId = id;
}
