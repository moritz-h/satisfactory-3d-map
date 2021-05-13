#include "Model.h"

#include <stdexcept>
#include <utility>
#include <vector>

#include <tiny_gltf.h>

#include "Utils/ResourceUtils.h"

namespace {
    std::vector<unsigned char> bufferSubset(const tinygltf::Buffer& buffer, const tinygltf::BufferView& bufferView) {
        const auto* begin = &buffer.data[bufferView.byteOffset];
        const auto* end = &buffer.data[bufferView.byteOffset + bufferView.byteLength];
        return std::vector<unsigned char>(begin, end);
    }

    glowl::Mesh::VertexData<unsigned char> loadVertexBuffer(
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

    glowl::Mesh::VertexData<unsigned char> loadPrimitiveAttribute(
        const tinygltf::Model& model, const tinygltf::Primitive& primitive, const std::string& attribute) {
        const auto& accessor = model.accessors[primitive.attributes.at(attribute)];
        return loadVertexBuffer(model, accessor);
    }

    glowl::Mesh::VertexDataList<unsigned char> parseVertexAttriutes(const tinygltf::Model& model,
        const tinygltf::Primitive& primitive, const std::vector<std::string>& attributes) {
        glowl::Mesh::VertexDataList<unsigned char> list;
        for (const auto& attreiute : attributes) {
            list.emplace_back(loadPrimitiveAttribute(model, primitive, attreiute));
        }
        return list;
    }

} // namespace

Satisfactory3DMap::Model::Model(const std::string& resourceName) : modelMx_(glm::mat4(1.0f)) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;
    const auto gltfResource = Satisfactory3DMap::getBinaryResource(resourceName);
    auto resourceBuffer = reinterpret_cast<const unsigned char*>(gltfResource.data());
    bool ret = loader.LoadBinaryFromMemory(
        &model, &err, &warn, resourceBuffer, gltfResource.size(), "", tinygltf::REQUIRE_ALL);
    if (!ret || !err.empty() || !warn.empty()) {
        throw std::runtime_error("Error loading model! Error: " + err + " Warning: " + warn);
    }
    if (model.nodes.size() != 1) {
        throw std::runtime_error("Model error: Only models with a single node are implemented!");
    }
    const auto& scene = model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
    const auto& node = model.nodes[scene.nodes[0]];
    const auto& mesh = model.meshes[node.mesh];
    const auto& primitive = mesh.primitives[0];

    // TODO parse node tree into model matrix.
    // TODO validation of input

    std::vector<std::string> requiredAttributes{"POSITION", "NORMAL", "TEXCOORD_0"};
    auto vertexInfoList = parseVertexAttriutes(model, primitive, requiredAttributes);

    const auto& idxAccessor = model.accessors[primitive.indices];
    const auto& idxBufferView = model.bufferViews[idxAccessor.bufferView];
    const auto& idxBuffer = model.buffers[idxBufferView.buffer];

    mesh_ = std::make_unique<glowl::Mesh>(
        vertexInfoList, bufferSubset(idxBuffer, idxBufferView), static_cast<GLenum>(idxAccessor.componentType));

    int texId = model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index;
    auto& image = model.images[texId];
    if (image.mimeType != "image/png") {
        throw std::runtime_error("Expect PNG image!");
    }

    glowl::TextureLayout texLayout(GL_SRGB8_ALPHA8, image.width, image.height, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1,
        {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
            {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}},
        {});
    tex_ = std::make_unique<glowl::Texture2D>("model", texLayout, image.image.data(), true);
}
