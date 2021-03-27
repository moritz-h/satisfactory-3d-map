#version 450

uniform sampler2D tex;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;
flat in int id;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    fragOutAlbedo = texture(tex, tex_coord);
    fragOutNormal = vec4(normalize(normal), 0.0f);
    fragOutId = id;
}