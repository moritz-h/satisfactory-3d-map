#version 450

flat out int instanceIdVert;

void main() {
    gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    instanceIdVert = gl_InstanceID;
}
