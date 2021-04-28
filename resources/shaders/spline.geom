#version 450

struct SplineSegment {
    vec4 p0;
    vec4 p1;
    vec4 tangent0;
    vec4 tangent1;
    int id;
    int type;
};

layout(std430, binding = 0) readonly buffer Segments { SplineSegment segments[]; };

layout(points) in;
layout(triangle_strip, max_vertices=32) out;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform int splineSubdivision;

flat in int instanceId[];

out vec3 position;
out vec3 normal;
flat out int id;
flat out int type;

void emitQuad(vec3 A, vec3 B, vec3 C, vec3 D, vec3 n0, vec3 n1) {
    //
    // C-----D
    // |  \  |
    // A-----B

    gl_Position = projMx * viewMx * vec4(A, 1.0f);
    position = A;
    normal = n0;
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(B, 1.0f);
    position = B;
    normal = n0;
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(C, 1.0f);
    position = C;
    normal = n1;
    EmitVertex();

    gl_Position = projMx * viewMx * vec4(D, 1.0f);
    position = D;
    normal = n1;
    EmitVertex();

    EndPrimitive();
}

void minimalGeometry(vec3 p0, vec3 right0, vec3 up0, vec3 p1, vec3 right1, vec3 up1) {
    // Calculate geometry points
    vec3 a00 = p0 - right0;
    vec3 a10 = p0 + right0;
    vec3 a01 = p1 - right1;
    vec3 a11 = p1 + right1;

    emitQuad(a00, a10, a01, a11, up0, up1);
}

void conveyorBeltGeometry(vec3 p0, vec3 right0, vec3 up0, vec3 p1, vec3 right1, vec3 up1) {
    // Calculate geometry points
    // G---F      C---B
    // |   |      |   |
    // |   E------D   |
    // |              |
    // H--------------A
    //
    //           (right, up)
    vec2 A = vec2( 0.9f, -0.4f);
    vec2 B = vec2( 0.9f,  0.25f);
    vec2 C = vec2( 0.75f, 0.25f);
    vec2 D = vec2( 0.75f, 0.0f);
    vec2 E = vec2(-0.75f, 0.0f);
    vec2 F = vec2(-0.75f, 0.25f);
    vec2 G = vec2(-0.9f,  0.25f);
    vec2 H = vec2(-0.9f, -0.4f);

    vec3 A0 = p0 + A.x * right0 + A.y * up0;
    vec3 B0 = p0 + B.x * right0 + B.y * up0;
    vec3 C0 = p0 + C.x * right0 + C.y * up0;
    vec3 D0 = p0 + D.x * right0 + D.y * up0;
    vec3 E0 = p0 + E.x * right0 + E.y * up0;
    vec3 F0 = p0 + F.x * right0 + F.y * up0;
    vec3 G0 = p0 + G.x * right0 + G.y * up0;
    vec3 H0 = p0 + H.x * right0 + H.y * up0;
    vec3 A1 = p1 + A.x * right1 + A.y * up1;
    vec3 B1 = p1 + B.x * right1 + B.y * up1;
    vec3 C1 = p1 + C.x * right1 + C.y * up1;
    vec3 D1 = p1 + D.x * right1 + D.y * up1;
    vec3 E1 = p1 + E.x * right1 + E.y * up1;
    vec3 F1 = p1 + F.x * right1 + F.y * up1;
    vec3 G1 = p1 + G.x * right1 + G.y * up1;
    vec3 H1 = p1 + H.x * right1 + H.y * up1;

    emitQuad(B0, A0, B1, A1, right0, right1);
    emitQuad(C0, B0, C1, B1, up0, up1);
    emitQuad(D0, C0, D1, C1, -right0, -right1);
    emitQuad(E0, D0, E1, D1, up0, up1);
    emitQuad(F0, E0, F1, E1, right0, right1);
    emitQuad(G0, F0, G1, F1, up0, up1);
    emitQuad(H0, G0, H1, G1, -right0, -right1);
    emitQuad(A0, H0, A1, H1, -up0, -up1);
}

void trackGeometry(vec3 p0, vec3 right0, vec3 up0, vec3 p1, vec3 right1, vec3 up1) {
    // Calculate geometry points
    //     E------D
    //     |      |
    // G---F      C---B
    // |              |
    // H--------------A
    //
    //           (right, up)
    vec2 A = vec2( 2.3f, 0.0f);
    vec2 B = vec2( 2.3f, 0.2f);
    vec2 C = vec2( 0.8f, 0.9f);
    vec2 D = vec2( 0.8f, 1.35f);
    vec2 E = vec2(-0.8f, 1.35f);
    vec2 F = vec2(-0.8f, 0.9f);
    vec2 G = vec2(-2.3f, 0.2f);
    vec2 H = vec2(-2.3f, 0.0f);


    vec3 A0 = p0 + A.x * right0 + A.y * up0;
    vec3 B0 = p0 + B.x * right0 + B.y * up0;
    vec3 C0 = p0 + C.x * right0 + C.y * up0;
    vec3 D0 = p0 + D.x * right0 + D.y * up0;
    vec3 E0 = p0 + E.x * right0 + E.y * up0;
    vec3 F0 = p0 + F.x * right0 + F.y * up0;
    vec3 G0 = p0 + G.x * right0 + G.y * up0;
    vec3 H0 = p0 + H.x * right0 + H.y * up0;
    vec3 A1 = p1 + A.x * right1 + A.y * up1;
    vec3 B1 = p1 + B.x * right1 + B.y * up1;
    vec3 C1 = p1 + C.x * right1 + C.y * up1;
    vec3 D1 = p1 + D.x * right1 + D.y * up1;
    vec3 E1 = p1 + E.x * right1 + E.y * up1;
    vec3 F1 = p1 + F.x * right1 + F.y * up1;
    vec3 G1 = p1 + G.x * right1 + G.y * up1;
    vec3 H1 = p1 + H.x * right1 + H.y * up1;

    emitQuad(B0, A0, B1, A1, right0, right1);
    emitQuad(C0, B0, C1, B1, normalize((C.y - B.y) * right0 + (B.x - C.x) * up0), normalize((C.y - B.y) * right1 + (B.x - C.x) * up1));
    emitQuad(D0, C0, D1, C1, right0, right1);
    emitQuad(E0, D0, E1, D1, up0, up1);
    emitQuad(F0, E0, F1, E1, -right0, -right1);
    emitQuad(G0, F0, G1, F1, normalize((F.y - G.y) * (-right0) + (F.x - G.x) * up0), normalize((F.y - G.y) * (-right1) + (F.x - G.x) * up1));
    emitQuad(H0, G0, H1, G1, -right0, -right1);
    emitQuad(A0, H0, A1, H1, -up0, -up1);
}

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

    // Output
    id = segments[segmentId].id;
    type = segments[segmentId].type;

    if (type == 1) { // ConveyorBelt
        conveyorBeltGeometry(p0, right0, up0, p1, right1, up1);
    } else if (type == 2) { // Pipe
        minimalGeometry(p0, right0, up0, p1, right1, up1);
    } else if (type == 3) { // Hyper
        minimalGeometry(p0, right0, up0, p1, right1, up1);
    } else if (type == 4) { // Track
        trackGeometry(p0, right0, up0, p1, right1, up1);
    } else {
        minimalGeometry(p0, right0, up0, p1, right1, up1);
    }
}
