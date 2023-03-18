#version 450

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

layout(location = 0) in vec3 in_position;
layout(location = 3) in vec2 in_texcoord0;

out vec3 vPosition;
out vec2 vTexCoord;

void main() {
    vec4 world_pos = modelMx * vec4(in_position, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    vPosition = world_pos.xyz / world_pos.w;
    vTexCoord = in_texcoord0;
}
