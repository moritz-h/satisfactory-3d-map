#include "MapLODglTFWriter.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <limits>
#include <span>
#include <string>

#include <fp16.h>
#include <glad/gl.h>
#include <spdlog/spdlog.h>
#include <stb_image_write.h>
#include <tiny_gltf.h>

#include "../OpenGL/Texture.h"
#include "Utils/ResourceUtils.h"

namespace {
    inline size_t padTo4(size_t size) {
        return (size + 3) & ~3;
    }

    /**
     * Write to Buffer object, return offset.
     */
    inline std::size_t writeBuffer(tinygltf::Buffer& buffer, std::span<const std::byte> data) {
        const auto offset = buffer.data.size();
        buffer.data.resize(offset + padTo4(data.size()));
        std::memcpy(buffer.data.data() + offset, data.data(), data.size());
        return offset;
    }

    /**
     * Write to first buffer of Model, create BufferView and return index of BufferView.
     */
    inline int writeBuffer(tinygltf::Model& m, std::span<const std::byte> data, int target = 0) {
        if (m.buffers.size() != 1) {
            throw std::runtime_error("Require model with exactly one buffer!");
        }
        const auto offset = writeBuffer(m.buffers[0], data);

        tinygltf::BufferView bufferView;
        bufferView.buffer = 0;
        bufferView.byteOffset = offset;
        bufferView.byteLength = data.size();
        bufferView.target = target;
        const auto bufferViewId = static_cast<int>(m.bufferViews.size());
        m.bufferViews.push_back(std::move(bufferView));
        return bufferViewId;
    }

    inline std::vector<std::byte> pngEncode(const std::vector<std::byte>& rgba, int x, int y) {
        constexpr auto writeFunc = [](void* context, void* data, int size) {
            auto* buf = static_cast<std::vector<std::byte>*>(context);
            auto* data_ptr = static_cast<std::byte*>(data);
            buf->insert(buf->end(), data_ptr, data_ptr + size);
        };
        std::vector<std::byte> buf;
        if (!stbi_write_png_to_func(writeFunc, &buf, x, y, 4, rgba.data(), 0)) {
            throw std::runtime_error("PNG error!");
        }
        return buf;
    }
} // namespace

Satisfactory3DMap::MapLODglTFWriter::MapLODglTFWriter(const std::shared_ptr<s::PakManager>& pakManager)
    : reader_(pakManager) {
    try {
        copyShader_ = std::make_unique<glowl::GLSLProgram>(
            glowl::GLSLProgram::ShaderSourceList{{glowl::GLSLProgram::ShaderType::Compute,
                Satisfactory3DMap::getStringResource("shaders/texture_copy.comp")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }
}

void Satisfactory3DMap::MapLODglTFWriter::save(const std::filesystem::path& path) {
    tinygltf::Model m;

    tinygltf::Asset asset;
    asset.version = "2.0";
    asset.generator = "tinygltf";
    asset.copyright = "Copyright (c) 2019-2026 Coffee Stain Studios AB";
    m.asset = asset;

    m.buffers.emplace_back();
    tinygltf::Scene scene;
    tinygltf::Node rootNode;
    rootNode.name = "Satisfactory-Map";
    rootNode.scale = {0.01, -0.01, 0.01}; // Convert to meter, flip y
    m.nodes.push_back(rootNode);          // Insert root node first
    scene.nodes.push_back(0);

    for (std::size_t meshIdx = 0; meshIdx < reader_.meshes().size(); meshIdx++) {
        const auto& ueMesh = reader_.meshes()[meshIdx];
        const std::string name =
            ueMesh.actorLabel + ";InstanceComponents[" + std::to_string(ueMesh.instanceComponentId) + "]";
        spdlog::info("Export mesh [{}/{}]: {}", meshIdx + 1, reader_.meshes().size(), name);

        const auto& LODResources = ueMesh.staticMesh->RenderData.LODResources[0];

        const auto& ueVertBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
        const auto& ueMeshBuffer = LODResources.VertexBuffers.StaticMeshVertexBuffer;
        if (ueVertBuffer.Stride != 12 || ueMeshBuffer.NumVertices != ueVertBuffer.NumVertices ||
            ueMeshBuffer.NumTexCoords < 1) {
            throw std::runtime_error("Unknown format of StaticMesh data not implemented!");
        }
        const auto& ueIndexBuffer = LODResources.IndexBuffer;
        if (ueIndexBuffer.b32Bit) {
            throw std::runtime_error("ueIndexBuffer.b32Bit not implemented!");
        }

        const auto& VertexData = ueVertBuffer.VertexData;
        const auto& TexcoordData = ueMeshBuffer.TexcoordData;
        const auto& IndexStorage = ueIndexBuffer.IndexStorage;
        int32_t VertexDataSize = VertexData.SerializedElementSize * VertexData.Num;
        int32_t TexcoordDataSize = TexcoordData.SerializedElementSize * TexcoordData.Num;
        int32_t IndexStorageSize = IndexStorage.SerializedElementSize * IndexStorage.Num;

        // Find min/max vertex position
        std::array<float, 3> vertMin{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()};
        std::array<float, 3> vertMax{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest()};
        const auto* vert_ptr = reinterpret_cast<const float*>(VertexData.data.data());
        for (std::size_t n = 0; n < VertexDataSize / 12; n++) {
            for (int i = 0; i < 3; i++) {
                vertMin[i] = std::min(vertMin[i], vert_ptr[3 * n + i]);
                vertMax[i] = std::max(vertMax[i], vert_ptr[3 * n + i]);
            }
        }

        // Convert tex coords from float16 to float32
        if (TexcoordDataSize != ueMeshBuffer.NumVertices * ueMeshBuffer.NumTexCoords * 2 * sizeof(uint16_t)) {
            throw std::runtime_error("Invalid tex coords");
        }
        std::vector<std::vector<float>> texcoords_data(ueMeshBuffer.NumTexCoords);
        const uint16_t* texcoord_ptr = reinterpret_cast<const uint16_t*>(TexcoordData.data.data());
        for (std::size_t t = 0; t < ueMeshBuffer.NumTexCoords; t++) {
            texcoords_data[t].resize(ueMeshBuffer.NumVertices * 2);
            for (std::size_t i = 0; i < ueMeshBuffer.NumVertices; i++) {
                const uint16_t x = texcoord_ptr[2 * (ueMeshBuffer.NumTexCoords * i + t) + 0];
                const uint16_t y = texcoord_ptr[2 * (ueMeshBuffer.NumTexCoords * i + t) + 1];
                texcoords_data[t][2 * i + 0] = fp16_ieee_to_fp32_value(x);
                texcoords_data[t][2 * i + 1] = fp16_ieee_to_fp32_value(y);
            }
        }

        // GLTF mesh
        tinygltf::Mesh mesh;
        mesh.primitives.emplace_back();
        auto& primitive = mesh.primitives[0];

        // Vertices
        const auto bufferViewVertexId =
            writeBuffer(m, std::as_bytes(std::span{VertexData.data}), TINYGLTF_TARGET_ARRAY_BUFFER);
        tinygltf::Accessor accessorVertex;
        accessorVertex.bufferView = bufferViewVertexId;
        accessorVertex.byteOffset = 0;
        accessorVertex.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessorVertex.count = VertexDataSize / 12;
        accessorVertex.type = TINYGLTF_TYPE_VEC3;
        accessorVertex.maxValues = {vertMax[0], vertMax[1], vertMax[2]};
        accessorVertex.minValues = {vertMin[0], vertMin[1], vertMin[2]};
        const auto accessorVertexId = static_cast<int>(m.accessors.size());
        m.accessors.push_back(accessorVertex);
        primitive.attributes["POSITION"] = accessorVertexId;

        // Tex Coords
        for (std::size_t t = 0; t < texcoords_data.size(); t++) {
            const auto bufferViewTexcoordId =
                writeBuffer(m, std::as_bytes(std::span{texcoords_data[t]}), TINYGLTF_TARGET_ARRAY_BUFFER);
            tinygltf::Accessor accessorTexCoord;
            accessorTexCoord.bufferView = bufferViewTexcoordId;
            accessorTexCoord.byteOffset = 0;
            accessorTexCoord.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
            accessorTexCoord.count = ueMeshBuffer.NumVertices;
            accessorTexCoord.type = TINYGLTF_TYPE_VEC2;
            const auto accessorTexCoordId = static_cast<int>(m.accessors.size());
            m.accessors.push_back(accessorTexCoord);
            primitive.attributes["TEXCOORD_" + std::to_string(t)] = accessorTexCoordId;

            break; // Currently only first tex coords are used.
        }

        // Indices
        const auto bufferViewIndexId =
            writeBuffer(m, std::as_bytes(std::span{IndexStorage.data}), TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER);
        tinygltf::Accessor accessorIndex;
        accessorIndex.bufferView = bufferViewIndexId;
        accessorIndex.byteOffset = 0;
        accessorIndex.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
        accessorIndex.count = IndexStorageSize / sizeof(uint16_t);
        accessorIndex.type = TINYGLTF_TYPE_SCALAR;
        const auto accessorIndexId = static_cast<int>(m.accessors.size());
        m.accessors.push_back(accessorIndex);
        primitive.indices = accessorIndexId;
        primitive.mode = TINYGLTF_MODE_TRIANGLES;

        // Texture (use OpenGL to unpack compressed texture)
        OGLTexture2D glTex(*ueMesh.baseColorTexture);
        auto texBufRGBA = decompressTexture(glTex.name(), glTex.sizeX(), glTex.sizeY());
        std::vector<std::byte> texBufPng = pngEncode(texBufRGBA, glTex.sizeX(), glTex.sizeY());
        const auto bufferViewTexId = writeBuffer(m, texBufPng);

        tinygltf::Image img;
        img.bufferView = bufferViewTexId;
        img.mimeType = "image/png";
        const auto imgId = static_cast<int>(m.images.size());
        m.images.push_back(img);

        tinygltf::Texture tex;
        tex.source = imgId;
        const auto texId = static_cast<int>(m.textures.size());
        m.textures.push_back(tex);

        tinygltf::Material mat;
        mat.pbrMetallicRoughness.baseColorTexture.index = texId;
        mat.doubleSided = true;
        const auto matId = static_cast<int>(m.materials.size());
        m.materials.push_back(mat);

        primitive.material = matId;
        const auto meshId = static_cast<int>(m.meshes.size());
        m.meshes.push_back(mesh);

        tinygltf::Node node;
        node.name = name;
        node.mesh = meshId;
        const auto nodeId = static_cast<int>(m.nodes.size());
        m.nodes.push_back(node);

        rootNode.children.push_back(nodeId);
    }

    m.nodes[0] = rootNode; // Update root node with children
    m.scenes.push_back(scene);
    m.defaultScene = 0;

    tinygltf::TinyGLTF gltf;
    gltf.WriteGltfSceneToFile(&m, path.string(), true, true, true, true);
}

std::vector<std::byte> Satisfactory3DMap::MapLODglTFWriter::decompressTexture(GLuint compressedTex, int width,
    int height) {
    if (!copyShader_) {
        throw std::runtime_error("Compute shader not initialized.");
    }

    GLuint outTex = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &outTex);
    glTextureParameteri(outTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(outTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(outTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(outTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(outTex, 1, GL_RGBA8, width, height);

    copyShader_->use();
    copyShader_->setUniform("width", width);
    copyShader_->setUniform("height", height);

    glBindTextureUnit(0, compressedTex);
    copyShader_->setUniform("srcTex", 0);
    glBindImageTexture(1, outTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    const GLuint groupsX = (width + 15) / 16;
    const GLuint groupsY = (height + 15) / 16;
    glDispatchCompute(groupsX, groupsY, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    std::vector<std::byte> outBuffer(width * height * 4);
    glGetTextureImage(outTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLsizei>(outBuffer.size()), outBuffer.data());

    glDeleteTextures(1, &outTex);

    return outBuffer;
}
