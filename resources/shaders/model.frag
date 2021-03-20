#version 450

uniform sampler2D tex;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

layout(location = 0) out vec4 fragColor0;
layout(location = 1) out int fragColor1;

void main() {
    fragColor0 = texture(tex, tex_coord);
    fragColor1 = 1000;
}
