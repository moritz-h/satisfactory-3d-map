#version 450

flat out int instanceId;

void main() {
    gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    instanceId = gl_InstanceID;
}
