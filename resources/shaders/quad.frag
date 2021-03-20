#version 450

uniform sampler2D tex;

in vec2 texCoords;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = texture(tex, texCoords);
}
