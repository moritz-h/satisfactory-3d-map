#version 450

layout(std430, binding = 0) readonly buffer Ids { int ids[]; };
layout(std430, binding = 1) readonly buffer Transformations { mat4 transformations[]; };

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
flat out int id;

void main() {
    vec4 world_pos = transformations[gl_InstanceID] * modelMx * vec4(in_position, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    normal = normalMx * in_normal;
    tex_coord = in_tex_coord;
    id = ids[gl_InstanceID];
}
