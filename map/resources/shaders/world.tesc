#version 450

layout(vertices = 1) out;

uniform int tessLevelInner;
uniform int tessLevelOuter;

flat in int instanceIdVert[];

patch out int instanceId;

void main() {
    gl_TessLevelInner[0] = tessLevelInner;
    gl_TessLevelInner[1] = tessLevelInner;
    gl_TessLevelOuter[0] = tessLevelOuter;
    gl_TessLevelOuter[1] = tessLevelOuter;
    gl_TessLevelOuter[2] = tessLevelOuter;
    gl_TessLevelOuter[3] = tessLevelOuter;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    instanceId = instanceIdVert[0];
}
