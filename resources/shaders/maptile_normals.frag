#version 450

uniform sampler2D texD;

in vec3 fNormal;
in vec2 fTexCoord;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec4 color = texture(texD, fTexCoord);

    fragOutAlbedo = vec4(color.rgb, 1.0f);
    fragOutNormal = vec4(fNormal, 0.0f);
    fragOutId = -2;
}
