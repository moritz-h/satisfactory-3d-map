#version 450

in vec3 position;
in vec3 normal;
flat in int id;
flat in int type;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec3 color = vec3(0.0f);
    if (type == 1) { // ConveyorBelt
        color = vec3(0.0f, 1.0f, 0.0f);
    } else if (type == 2) { // Pipe
        color = vec3(1.0f, 0.5f, 0.0f);
    } else if (type == 3) { // Hyper
        color = vec3(1.0f, 1.0f, 0.0f);
    } else if (type == 4) { // Track
        color = vec3(0.0f, 0.0f, 1.0f);
    }

    fragOutAlbedo = vec4(color, 1.0f);
    fragOutNormal = vec4(normalize(normal), 0.0f);
    fragOutId = id;
}
