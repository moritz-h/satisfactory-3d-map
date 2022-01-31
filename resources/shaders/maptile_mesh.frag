#version 450

in vec3 position;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    fragOutAlbedo = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    fragOutNormal = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    fragOutId = -2;
}
