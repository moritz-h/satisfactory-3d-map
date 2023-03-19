#version 450

// PBR lighting from:
// https://github.com/KhronosGroup/glTF-Sample-Viewer
// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#appendix-b-brdf-implementation

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texDepth;

uniform mat4 invProjMx;
uniform mat4 invViewMx;

uniform float roughness;
uniform float metallic;

in vec2 texCoords;

layout(location = 0) out vec4 fragColor;

const float pi = 3.14159265358979323846;

float clampedDot(vec3 x, vec3 y) {
    return clamp(dot(x, y), 0.0, 1.0);
}

vec3 F_Schlick(vec3 f0, vec3 f90, float VdotH) {
    return f0 + (f90 - f0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
}

float V_GGX(float NdotL, float NdotV, float alphaRoughness) {
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;

    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);

    float GGX = GGXV + GGXL;
    if (GGX > 0.0) {
        return 0.5 / GGX;
    }
    return 0.0;
}

float D_GGX(float NdotH, float alphaRoughness) {
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;
    float f = (NdotH * NdotH) * (alphaRoughnessSq - 1.0) + 1.0;
    return alphaRoughnessSq / (pi * f * f);
}

vec3 BRDF_lambertian(vec3 f0, vec3 f90, vec3 diffuseColor, float VdotH) {
    return (1.0 - F_Schlick(f0, f90, VdotH)) * (diffuseColor / pi);
}

vec3 BRDF_specularGGX(vec3 f0, vec3 f90, float alphaRoughness, float VdotH, float NdotL, float NdotV, float NdotH) {
    vec3 F = F_Schlick(f0, f90, VdotH);
    float Vis = V_GGX(NdotL, NdotV, alphaRoughness);
    float D = D_GGX(NdotH, alphaRoughness);

    return F * Vis * D;
}

void main() {
    vec4 baseColor = texture(texAlbedo, texCoords);

    // Discard if fragment is fully transparent (default clear color)
    if (baseColor.a == 0.0f) {
        discard;
    }

    vec3 normal = texture(texNormal, texCoords).rgb;

    // Passthrough base color if normal is zero
    if (length(normal) == 0.0f) {
        fragColor = baseColor;
        return;
    }

    // Determine world coordinates from pixel position and depth image.
    vec3 xyz_screen = vec3(texCoords, texture(texDepth, texCoords).r);
    vec3 xyz_ndc = xyz_screen * vec3(2.0) - vec3(1.0);
    vec4 world_pos = invViewMx * invProjMx * vec4(xyz_ndc, 1.0);
    world_pos /= world_pos.w;

    vec3 camera  = invViewMx[3].xyz / invViewMx[3].w; // invViewMx[3] is same as invViewMx * vec4(0.0, 0.0, 0.0, 1.0)

    vec3 V = normalize(camera - world_pos.xyz);
    vec3 L = V; // Head light
    vec3 N = normalize(normal);
    vec3 H = normalize(L + V);

    vec3 f0 = vec3(0.04);
    vec3 albedoColor = mix(baseColor.rgb * (vec3(1.0) - f0), vec3(0), metallic);
    f0 = mix(f0, baseColor.rgb, metallic);
    float alphaRoughness = roughness * roughness;

    float reflectance = max(max(f0.r, f0.g), f0.b);
    vec3 f90 = vec3(clamp(reflectance * 50.0, 0.0, 1.0));

    float NdotL = clampedDot(N, L);
    float NdotV = clampedDot(N, V);
    float NdotH = clampedDot(N, H);
    float LdotH = clampedDot(L, H);
    float VdotH = clampedDot(V, H);

    float intensity = pi;

    vec3 f_diffuse = vec3(0);
    vec3 f_specular = vec3(0);

    if (NdotL > 0.0 || NdotV > 0.0) {
        f_diffuse += intensity * NdotL * BRDF_lambertian(f0, f90, albedoColor, VdotH);
        f_specular += intensity * NdotL * BRDF_specularGGX(f0, f90, alphaRoughness, VdotH, NdotL, NdotV, NdotH);
    }

    vec3 color = f_diffuse + f_specular;

    // gamma correction
    color = pow(color, vec3(1.0 / 2.2f));

    // fade out depth clipping
    const float fade_start = 0.999995f;
    if (xyz_screen.z > fade_start && xyz_screen.z < 1.0f) {
        float transparency = (xyz_screen.z - fade_start) / (1.0f - fade_start);
        color = mix(color, vec3(0.2f, 0.2f, 0.2f), transparency);
    }

    fragColor = vec4(color, 1.0f);
}
