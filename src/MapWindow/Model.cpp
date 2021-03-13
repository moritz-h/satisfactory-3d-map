#include "Model.h"

#include <tiny_gltf.h>

#include "Utils/ResourceUtils.h"

namespace {
    template<typename T>
    std::vector<T> loadBuffer(
        const tinygltf::Model& model, const tinygltf::Accessor& accessor, int componentType, int type) {
        const auto& bufferView = model.bufferViews[accessor.bufferView];
        if (accessor.componentType != componentType || accessor.type != type) {
            throw std::runtime_error("Accessor does not contain expected type!");
        }

        if (bufferView.byteLength != accessor.count * tinygltf::GetNumComponentsInType(type) * sizeof(T)) {
            throw std::runtime_error("Unexpected BufferView size!");
        }

        auto buffer = model.buffers[bufferView.buffer].data;
        const T* begin = reinterpret_cast<T*>(&buffer[bufferView.byteOffset]);
        const T* end = reinterpret_cast<T*>(&buffer[bufferView.byteOffset + bufferView.byteLength]);
        return std::vector<T>(begin, end);
    }

    std::vector<float> loadFloatPrimitiveAttribute(
        const tinygltf::Model& model, const tinygltf::Primitive& primitive, const std::string& attribute, int type) {
        const auto& accessor = model.accessors[primitive.attributes.at(attribute)];
        return loadBuffer<float>(model, accessor, GL_FLOAT, type);
    }
} // namespace

Satisfactory3DMap::Model::Model(const std::string& resourceName) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    const auto gltfResource = Satisfactory3DMap::getBinaryResource(resourceName);
    auto resourceBuffer = reinterpret_cast<const unsigned char*>(gltfResource.data());
    bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, resourceBuffer, gltfResource.size());
    if (!ret || !err.empty() || !warn.empty()) {
        throw std::runtime_error("Error loading model! Error: " + err + " Warning: " + warn);
    }

    // TODO validation of input
    const auto& scene = model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
    const auto& node = model.nodes[scene.nodes[0]];
    const auto& mesh = model.meshes[node.mesh];
    const auto& primitive = mesh.primitives[0];

    auto position = loadFloatPrimitiveAttribute(model, primitive, "POSITION", TINYGLTF_TYPE_VEC3);
    auto normal = loadFloatPrimitiveAttribute(model, primitive, "NORMAL", TINYGLTF_TYPE_VEC3);
    auto texcoord = loadFloatPrimitiveAttribute(model, primitive, "TEXCOORD_0", TINYGLTF_TYPE_VEC2);
    auto indices =
        loadBuffer<unsigned short>(model, model.accessors[primitive.indices], GL_UNSIGNED_SHORT, TINYGLTF_TYPE_SCALAR);

    mesh_ = std::make_unique<glowl::Mesh>(std::vector<std::vector<float>>{position, normal, texcoord}, indices,
        std::vector<glowl::VertexLayout>{
            {12, {{3, GL_FLOAT, GL_FALSE, 0}}}, {12, {{3, GL_FLOAT, GL_FALSE, 0}}}, {8, {{2, GL_FLOAT, GL_FALSE, 0}}}},
        GL_UNSIGNED_SHORT);

    int texId = model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index;
    auto& image = model.images[texId];
    if (image.mimeType != "image/png") {
        throw std::runtime_error("Expect PNG image!");
    }

    glowl::TextureLayout texLayout(GL_RGBA8, image.width, image.height, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1,
        {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
            {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}},
        {});
    tex_ = std::make_unique<glowl::Texture2D>("box", texLayout, image.image.data());
}
