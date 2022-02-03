#version 450

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texcoord0;
layout(location = 2) in vec2 in_texcoord1;

out vec3 position;
out vec2 texCoord;

void main() {
    vec4 world_pos = modelMx * vec4(in_position, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    texCoord = in_texcoord0;
}
