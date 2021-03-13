#version 450

uniform sampler2D tex;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

out vec4 fragColor;

void main() {
    fragColor = texture(tex, tex_coord);
}
