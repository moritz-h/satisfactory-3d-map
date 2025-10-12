#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vPosition[];
in vec2 vTexCoord[];

out vec3 fNormal;
out vec2 fTexCoord;

void main() {
    vec3 normal = normalize(cross(vPosition[1] - vPosition[0], vPosition[2] - vPosition[0]));

    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        fNormal = -normal;
        fTexCoord = vTexCoord[i];
        EmitVertex();
    }
    EndPrimitive();
}
