#version 450

uniform mat4 projMxQuad;

layout(location = 0) in vec2 in_position;

out vec2 texCoords;

void main() {
    gl_Position = projMxQuad * vec4(in_position, 0.0f, 1.0f);
    texCoords = in_position;
}
