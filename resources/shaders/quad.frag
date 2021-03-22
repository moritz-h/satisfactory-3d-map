#version 450

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texDepth;

uniform mat4 invProjMx;
uniform mat4 invViewMx;

uniform vec3 ambient = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 specular = vec3(1.0f, 1.0f, 1.0f);

uniform float k_amb = 0.25f;
uniform float k_diff = 0.7f;
uniform float k_spec = 0.05f;
uniform float k_exp = 200.0f;

in vec2 texCoords;

layout(location = 0) out vec4 fragColor;

const float pi = 3.14159265358979323846;

vec3 blinnPhong(vec3 n, vec3 l, vec3 v, vec3 col) {
    vec3 color = vec3(0.0f);

    // ambient
    color += k_amb * ambient * col;

    // diffuse
    float NdotL = abs(dot(n, l)); // max(0.0f, dot(n, l));
    color += k_diff * diffuse * NdotL * col;

    // specular
    vec3 h = normalize(l + v);
    float NdotH = abs(dot(n, h)); // max(0.0f, dot(n, h));
    color += k_spec * specular * ((k_exp + 2.0f) / (2.0f * pi)) * pow(NdotH, k_exp);
    return color;
}

void main() {
    // Determine world coordinates from pixel position and depth image.
    vec3 xyz_screen = vec3(texCoords, texture(texDepth, texCoords).r);
    vec3 xyz_ndc = xyz_screen * vec3(2.0) - vec3(1.0);
    vec4 world_pos = invViewMx * invProjMx * vec4(xyz_ndc, 1.0);
    world_pos /= world_pos.w;

    vec3 camera  = invViewMx[3].xyz / invViewMx[3].w; // invViewMx[3] is same as invViewMx * vec4(0.0, 0.0, 0.0, 1.0)

    vec3 albedo = texture(texAlbedo, texCoords).rgb;
    vec3 normal = texture(texNormal, texCoords).rgb;

    vec3 viewVec = normalize(camera - world_pos.xyz);

    vec3 color = blinnPhong(normalize(normal), viewVec, viewVec, albedo);

    // gamma correction
    fragColor = vec4(pow(color, vec3(1.0 / 2.2f)), 1.0f);
}
