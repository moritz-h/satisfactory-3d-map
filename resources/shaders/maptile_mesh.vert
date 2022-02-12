#version 450

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_tangent_x;
layout(location = 2) in vec4 in_tangent_z;
layout(location = 3) in vec2 in_texcoord0;
layout(location = 4) in vec2 in_texcoord1;

out vec3 position;
out vec2 texCoord;
out vec4 tangent_x;
out vec4 tangent_z;

void main() {
    vec4 world_pos = modelMx * vec4(in_position, 1.0f);
    gl_Position = projMx * viewMx * world_pos;
    position = world_pos.xyz;
    tangent_x = vec4(mat3(modelMx) * in_tangent_x.xyz, in_tangent_x.w);
    tangent_z = vec4(inverse(transpose(mat3(modelMx))) * in_tangent_z.xyz, in_tangent_z.w);
    texCoord = in_texcoord0;
}
