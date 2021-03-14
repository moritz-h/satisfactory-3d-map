#version 450

layout(std430, binding = 0) readonly buffer Positions { vec4 positions[]; };

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec3 position;
out vec3 normal;
out vec2 tex_coord;

void main() {
    vec3 pos = in_position + positions[gl_InstanceID].xyz;
    gl_Position = projMx * viewMx * modelMx * vec4(pos, 1.0f);
    position = pos;
    normal = in_normal;
    tex_coord = in_tex_coord;
}
