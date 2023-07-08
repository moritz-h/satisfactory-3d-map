#version 450

uniform sampler2D texD;
uniform sampler2D texN;

in vec3 position;
in vec4 tangent_x;
in vec4 tangent_z;
in vec2 texCoord;

layout(location = 0) out vec4 fragOutAlbedo;
layout(location = 1) out vec4 fragOutNormal;
layout(location = 2) out int fragOutId;

void main() {
    vec4 color = texture(texD, texCoord);

    // Tangent space matrix
    vec3 T = normalize(tangent_x.xyz);
    vec3 N = normalize(tangent_z.xyz);
    vec3 B = normalize(tangent_z.w * cross(T, N));
    mat3 TBN = mat3(T, B, N);

    // Normal map in tangent space
    vec2 normalXY = texture(texN, texCoord).rg * 2.0f - 1.0f;
    float normalZ = sqrt(1.0f - normalXY.x * normalXY.x - normalXY.y * normalXY.y);
    vec3 normal = normalize(vec3(normalXY, normalZ));

    // Map to world space
    normal = normalize(TBN * normal);

    fragOutAlbedo = vec4(color.rgb, 1.0f);
    fragOutNormal = vec4(normal, 0.0f);
    fragOutId = -2;
}
