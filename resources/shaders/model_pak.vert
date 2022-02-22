#version 450

layout(std430, binding = 0) readonly buffer Ids { int ids[]; };
layout(std430, binding = 1) readonly buffer Transformations { mat4 transformations[]; };

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
    // Transform coords
    vec3 pos = vec3(0.01f * in_position.x, -0.01f * in_position.y, 0.01f * in_position.z);
    vec3 tx = vec3(in_tangent_x.x, -in_tangent_x.y, in_tangent_x.z);
    vec3 tz = vec3(in_tangent_z.x, -in_tangent_z.y, in_tangent_z.z);

    vec4 world_pos = transformations[gl_InstanceID] * modelMx * vec4(pos, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    mat3 transformMx = mat3(transformations[gl_InstanceID]);
    tangent_x = vec4(transformMx * mat3(modelMx) * tx, in_tangent_x.w);
    tangent_z = vec4(transpose(inverse(transformMx)) * normalMx * tz, in_tangent_z.w);
    texCoord = in_texcoord0; // Seems to fit best for color and normals.
    id = ids[gl_InstanceID];
}
