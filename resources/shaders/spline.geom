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
layout(triangle_strip, max_vertices=4) out;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform int splineSubdivision;

flat in int instanceId[];

out vec3 position;
out vec3 normal;
flat out int id;

void main() {
    int segmentId = instanceId[0] / splineSubdivision;
    int subdivisionId = instanceId[0] % splineSubdivision;

    // Bezier control points
    vec3 b0 = segments[segmentId].p0.xyz;
    vec3 b3 = segments[segmentId].p1.xyz;
    // Unreal Engine seems to use cubic Hermite splines, convert to Bezier control points.
    vec3 b1 = b0 + segments[segmentId].tangent0.xyz / 3.0f;
    vec3 b2 = b3 - segments[segmentId].tangent1.xyz / 3.0f;

    // Local subdivision line start and end parameter
    float t0 = float(subdivisionId) / float(splineSubdivision);
    float t1 = float(subdivisionId + 1) / float(splineSubdivision);

    // De Casteljau P0
    vec3 b01_0 = mix(b0, b1, t0);
    vec3 b11_0 = mix(b1, b2, t0);
    vec3 b21_0 = mix(b2, b3, t0);

    vec3 b02_0 = mix(b01_0, b11_0, t0);
    vec3 b12_0 = mix(b11_0, b21_0, t0);

    vec3 p0 = mix(b02_0, b12_0, t0);
    vec3 forward0 = normalize(b12_0 - b02_0);
    vec3 right0 = normalize(cross(forward0, vec3(0.0f, 0.0f, 1.0f)));
    vec3 up0 = normalize(cross(right0, forward0));

    // De Casteljau P1
    vec3 b01_1 = mix(b0, b1, t1);
    vec3 b11_1 = mix(b1, b2, t1);
    vec3 b21_1 = mix(b2, b3, t1);

    vec3 b02_1 = mix(b01_1, b11_1, t1);
    vec3 b12_1 = mix(b11_1, b21_1, t1);

    vec3 p1 = mix(b02_1, b12_1, t1);
    vec3 forward1 = normalize(b12_1 - b02_1);
    vec3 right1 = normalize(cross(forward1, vec3(0.0f, 0.0f, 1.0f)));
    vec3 up1 = normalize(cross(right1, forward1));

    // Calculate geometry points
    vec3 a00 = p0 - right0;
    vec3 a10 = p0 + right0;
    vec3 a01 = p1 - right1;
    vec3 a11 = p1 + right1;

    // Output
    id = ids[segmentId];

    gl_Position = projMx * viewMx * vec4(a00, 1.0f);
    position = a00;
    normal = up0;
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(a10, 1.0f);
    position = a10;
    normal = up0;
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(a01, 1.0f);
    position = a01;
    normal = up1;
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(a11, 1.0f);
    position = a11;
    normal = up1;
    EmitVertex();

    EndPrimitive();
}
