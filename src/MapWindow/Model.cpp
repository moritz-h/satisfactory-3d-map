#include "Model.h"

#include <utility>

#include <tiny_gltf.h>

#include "Utils/ResourceUtils.h"

namespace {
    std::vector<unsigned char> bufferSubset(const tinygltf::Buffer& buffer, const tinygltf::BufferView& bufferView) {
        const auto* begin = &buffer.data[bufferView.byteOffset];
        const auto* end = &buffer.data[bufferView.byteOffset + bufferView.byteLength];
        return std::vector<unsigned char>(begin, end);
    }

    glowl::Mesh::VertexInfo<unsigned char> loadVertexBuffer(
        const tinygltf::Model& model, const tinygltf::Accessor& accessor) {
        const auto& bufferView = model.bufferViews[accessor.bufferView];

        auto componentSize = tinygltf::GetComponentSizeInBytes(static_cast<uint32_t>(accessor.componentType));
        auto numComponents = tinygltf::GetNumComponentsInType(accessor.type);
        auto stride = componentSize * numComponents;

        if (stride * accessor.count != bufferView.byteLength || accessor.byteOffset != 0) {
            throw std::runtime_error("Only continuous buffer implemented!");
        }

        std::vector<unsigned char> data = bufferSubset(model.buffers[bufferView.buffer], bufferView);
        glowl::VertexLayout layout{
            stride, {{numComponents, static_cast<GLenum>(accessor.componentType), accessor.normalized, 0}}};

        return std::make_pair(data, layout);
    }

    glowl::Mesh::VertexInfo<unsigned char> loadPrimitiveAttribute(
        const tinygltf::Model& model, const tinygltf::Primitive& primitive, const std::string& attribute) {
        const auto& accessor = model.accessors[primitive.attributes.at(attribute)];
        return loadVertexBuffer(model, accessor);
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

    auto position = loadPrimitiveAttribute(model, primitive, "POSITION");
    auto normal = loadPrimitiveAttribute(model, primitive, "NORMAL");
    auto texcoord = loadPrimitiveAttribute(model, primitive, "TEXCOORD_0");

    const auto& idxAccessor = model.accessors[primitive.indices];
    const auto& idxBufferView = model.bufferViews[idxAccessor.bufferView];
    const auto& idxBuffer = model.buffers[idxBufferView.buffer];

    mesh_ = std::make_unique<glowl::Mesh>(glowl::Mesh::VertexInfoList<unsigned char>{position, normal, texcoord},
        bufferSubset(idxBuffer, idxBufferView), static_cast<GLenum>(idxAccessor.componentType));

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
