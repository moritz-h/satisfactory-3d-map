#version 450

layout(quads, equal_spacing, ccw) in;

uniform sampler2D texHeight;

uniform mat4 projMx;
uniform mat4 viewMx;

uniform int numInstancesX;
uniform int numInstancesY;

uniform vec3 posTransferFuncA;
uniform vec3 posTransferFuncB;
uniform vec2 sizeTexHeight;

patch in int instanceId;

out vec3 position;
out vec3 normal;
out vec2 texCoords;

void main() {
    // index of current tile
    int tx = instanceId % (numInstancesX);
    int ty = instanceId / (numInstancesX);
    float u = (float(tx) + gl_TessCoord.x) / float(numInstancesX);
    float v = (float(ty) + gl_TessCoord.y) / float(numInstancesY);

    // tex coords with (0,0) top left
    vec2 tc = vec2(u, 1.0f - v);

    vec3 world_pos = vec3(u, v, texture(texHeight, tc).r) * posTransferFuncA + posTransferFuncB;

    // calculate normal
    float height_xm = textureOffset(texHeight, tc, ivec2(-1, 0)).r * posTransferFuncA.z + posTransferFuncB.z;
    float height_xp = textureOffset(texHeight, tc, ivec2(1, 0)).r * posTransferFuncA.z + posTransferFuncB.z;
    float height_ym = textureOffset(texHeight, tc, ivec2(0, 1)).r * posTransferFuncA.z + posTransferFuncB.z;
    float height_yp = textureOffset(texHeight, tc, ivec2(0, -1)).r * posTransferFuncA.z + posTransferFuncB.z;
    float height_dx = (height_xp - height_xm) / 2.0f;
    float height_dy = (height_yp - height_ym) / 2.0f;
    vec2 lengthScale = posTransferFuncA.xy / sizeTexHeight;
    vec3 n = normalize(cross(vec3(lengthScale.x, 0.0f, height_dx), vec3(0.0f, lengthScale.y, height_dy)));

    gl_Position = projMx * viewMx * vec4(world_pos, 1.0f);
    position = world_pos;
    normal = n;
    texCoords = tc;
}
