#version 450

uniform sampler2D texMap00;
uniform sampler2D texMap01;
uniform sampler2D texMap10;
uniform sampler2D texMap11;

in vec3 position;
in vec3 normal;
in vec2 texCoords;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

vec4 readMapTexture(vec2 coords) {
    if (coords.x <= 0.5f) {
        if (coords.y <= 0.5f) {
            return texture(texMap00, texCoords * 2.0f);
        } else {
            return texture(texMap01, vec2(texCoords.x * 2.0f, texCoords.y * 2.0f - 1.0f));
        }
    } else {
        if (coords.y <= 0.5f) {
            return texture(texMap10, vec2(texCoords.x * 2.0f - 1.0f, texCoords.y * 2.0f));
        } else {
            return texture(texMap11, texCoords * 2.0f - 1.0f);
        }
    }
}

void main() {
    fragOutAlbedo = readMapTexture(texCoords);
    fragOutNormal = vec4(normalize(normal), 0.0f);
    fragOutId = -2;
}
