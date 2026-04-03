#version 450

struct PowerLineInstance {
    int listOffset;
    int _padding1_;
    int _padding2_;
    int _padding3_;
    vec4 p0;
    vec4 p1;
};

layout(std430, binding = 0) readonly buffer Ids { int ids[]; };
layout(std430, binding = 1) readonly buffer Transformations { mat4 transformations[]; };
layout(std430, binding = 2) readonly buffer Instances { PowerLineInstance instances[]; };

uniform mat4 projMx;
uniform mat4 viewMx;

out vec3 position;
out vec3 normal;
flat out int id;

const vec3 vert[16] = vec3[](
    vec3(0.0f, -0.4f, -1.0f),
    vec3(1.0f, -0.4f, -1.0f),
    vec3(0.0f,  0.4f, -1.0f),
    vec3(1.0f,  0.4f, -1.0f),
    vec3(0.0f,  1.0f, -0.4f),
    vec3(1.0f,  1.0f, -0.4f),
    vec3(0.0f,  1.0f,  0.4f),
    vec3(1.0f,  1.0f,  0.4f),
    vec3(0.0f,  0.4f,  1.0f),
    vec3(1.0f,  0.4f,  1.0f),
    vec3(0.0f, -0.4f,  1.0f),
    vec3(1.0f, -0.4f,  1.0f),
    vec3(0.0f, -1.0f,  0.4f),
    vec3(1.0f, -1.0f,  0.4f),
    vec3(0.0f, -1.0f, -0.4f),
    vec3(1.0f, -1.0f, -0.4f)
);

void main() {
    const PowerLineInstance instance = instances[gl_InstanceID];
    const int actorListIdx = instance.listOffset;

    vec3 p0 = instance.p0.xyz;
    vec3 p1 = instance.p1.xyz;
    float width = 0.35f;

    vec3 xAxis = p1 - p0;
    float len = length(xAxis);
    xAxis = normalize(xAxis);

    vec3 up = (abs(xAxis.z) < 0.9f) ? vec3(0.0f, 0.0f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);
    vec3 yAxis = normalize(cross(up, xAxis));
    vec3 zAxis = normalize(cross(xAxis, yAxis));

    vec3 local = vert[gl_VertexID % 16];
    float halfWidth = 0.5 * width;

    vec3 pos = p0 + xAxis * (local.x * len) + yAxis * (local.y * halfWidth) + zAxis * (local.z * halfWidth);

    vec3 normal_local = normalize(vec3(0.0f, local.y, local.z));

    gl_Position = projMx * viewMx * vec4(pos, 1.0f);
    position = pos;
    normal = normalize(yAxis * normal_local.y + zAxis * normal_local.z);
    id = ids[actorListIdx];
}
