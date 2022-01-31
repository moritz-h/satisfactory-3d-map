#version 450

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;
uniform mat3 normalMx;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coord;

out vec3 position;
out vec3 normal;
out vec2 tex_coord;

void main() {
    vec4 world_pos = modelMx * vec4(in_position.x, -in_position.z, in_position.y, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    normal = normalMx * vec3(in_normal.x, -in_normal.z, in_normal.y);
    tex_coord = in_tex_coord;
}
