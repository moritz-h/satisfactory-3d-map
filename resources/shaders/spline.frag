#version 450

flat in int id;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec3 normal = vec3(0.0f, 0.0f, 1.0f);

    fragOutAlbedo = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    fragOutNormal = vec4(normalize(normal), 0.0f);
    fragOutId = id;
}
