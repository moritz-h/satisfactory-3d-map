#version 450

layout(std430, binding = 0) readonly buffer Ids { int ids[]; };
layout(std430, binding = 1) readonly buffer Transformations { mat4 transformations[]; };
layout(std430, binding = 2) readonly buffer ListOffsets { int listOffsets[]; };

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;
uniform mat3 normalMx;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_tangent_x;
layout(location = 2) in vec4 in_tangent_z;
layout(location = 3) in vec2 in_texcoord0;
layout(location = 4) in vec2 in_texcoord1;

out vec3 position;
out vec4 tangent_x;
out vec4 tangent_z;
out vec2 texCoord;
flat out int id;

void main() {
    const int actorListIdx = listOffsets[gl_InstanceID];

    vec4 uePosition = modelMx * vec4(in_position, 1.0f);
    vec3 ueTx = mat3(modelMx) * in_tangent_x.xyz;
    vec3 ueTz = normalMx * in_tangent_z.xyz;
    // Transform coords
    vec4 glPos = vec4(0.01f * uePosition.x, -0.01f * uePosition.y, 0.01f * uePosition.z, uePosition.w);
    vec3 glTx = vec3(ueTx.x, -ueTx.y, ueTx.z);
    vec3 glTz = vec3(ueTz.x, -ueTz.y, ueTz.z);

    vec4 world_pos = transformations[actorListIdx] * glPos;
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    mat3 transformMx3x3 = mat3(transformations[actorListIdx]);
    tangent_x = vec4(transformMx3x3 * glTx, in_tangent_x.w);
    tangent_z = vec4(transpose(inverse(transformMx3x3)) * glTz, in_tangent_z.w);
    texCoord = in_texcoord0; // Seems to fit best for color and normals.
    id = ids[actorListIdx];
}
