#version 450

struct SplineSegment {
    vec4 p0;
    vec4 p1;
    vec4 tangent0;
    vec4 tangent1;
};

layout(std430, binding = 0) readonly buffer Ids { int ids[]; };
layout(std430, binding = 1) readonly buffer Segments { SplineSegment segments[]; };

layout(points) in;
layout(line_strip, max_vertices=2) out;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform int splineSubdivision;

flat in int instanceId[];

flat out int id;

vec3 deCasteljau(vec3 b0, vec3 b1, vec3 b2, vec3 b3, float t) {
    vec3 b01 = mix(b0, b1, t);
    vec3 b11 = mix(b1, b2, t);
    vec3 b21 = mix(b2, b3, t);

    vec3 b02 = mix(b01, b11, t);
    vec3 b12 = mix(b11, b21, t);

    return mix(b02, b12, t);
}

void main() {
    int segmentId = instanceId[0] / splineSubdivision;
    int subdivisionId = instanceId[0] % splineSubdivision;

    float t0 = float(subdivisionId) / float(splineSubdivision);
    float t1 = float(subdivisionId + 1) / float(splineSubdivision);

    vec3 b0 = segments[segmentId].p0.xyz;
    vec3 b3 = segments[segmentId].p1.xyz;
    // Unreal Engine seems to use cubic Hermite splines, convert to Bezier control points.
    vec3 b1 = b0 + segments[segmentId].tangent0.xyz / 3.0f;
    vec3 b2 = b3 - segments[segmentId].tangent1.xyz / 3.0f;

    id = ids[segmentId];

    gl_Position = projMx * viewMx * vec4(deCasteljau(b0, b1, b2, b3, t0), 1.0f);
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(deCasteljau(b0, b1, b2, b3, t1), 1.0f);
    EmitVertex();

    EndPrimitive();
}
