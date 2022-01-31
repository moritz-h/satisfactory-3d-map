#version 450

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

layout(location = 0) in vec3 in_position;

out vec3 position;

void main() {
    vec4 world_pos = modelMx * vec4(in_position, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
}
