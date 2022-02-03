#version 450

uniform sampler2D tex;

in vec3 position;
in vec2 texCoord;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec4 color = texture(tex, texCoord);
    fragOutAlbedo = vec4(color.rgb, 1.0f);
    fragOutNormal = vec4(0.0f, 0.0f, 1.0f, 0.0f);
    fragOutId = -2;
}
