#version 450

layout(std430, binding = 0) readonly buffer Ids { int ids[]; };
layout(std430, binding = 1) readonly buffer Transformations { mat4 transformations[]; };
layout(std430, binding = 2) readonly buffer ListOffsets { int listOffsets[]; };

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
    const int actorListIdx = listOffsets[gl_InstanceID];

    vec3 in_position_LH = vec3(in_position.x, -in_position.y, in_position.z);
    vec3 in_normal_LH = vec3(in_normal.x, -in_normal.y, in_normal.z);

    vec4 world_pos = transformations[actorListIdx] * modelMx * vec4(in_position_LH, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    normal = transpose(inverse(mat3(transformations[actorListIdx]))) * normalMx * in_normal_LH;
    tex_coord = in_tex_coord;
    id = ids[actorListIdx];
}
