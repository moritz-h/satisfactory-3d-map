#include "GlowlFactory.h"

#include "SatisfactorySave/Pak/Serialization/StaticMesh.h"

std::shared_ptr<glowl::Mesh> Satisfactory3DMap::makeGlowlMesh(const Satisfactory3DMap::StaticMesh& mesh) {
    const auto& LODResources = mesh.renderData().LODResources[0];

    const auto& vertexBuffers = LODResources.VertexBuffers;
    const auto& ueVertBuffer = vertexBuffers.PositionVertexBuffer;
    const auto& ueMeshBuffer = vertexBuffers.StaticMeshVertexBuffer;
    if (ueVertBuffer.Stride != 12 || ueMeshBuffer.NumVertices != ueVertBuffer.NumVertices ||
        ueMeshBuffer.NumTexCoords < 1 || ueMeshBuffer.TexcoordData.SerializedElementSize != 4) {
        throw std::runtime_error("Unknown format of StaticMesh data not implemented!");
    }

    const auto& ueIndexBuffer = LODResources.IndexBuffer;
    if (ueIndexBuffer.b32Bit) {
        throw std::runtime_error("ueIndexBuffer.b32Bit not implemented!");
    }
    if (ueIndexBuffer.IndexStorage.SerializedElementSize != 1) {
        throw std::runtime_error("ueIndexBuffer.IndexStorage.SerializedElementSize != 1 not implemented!");
    }

    const auto& VertexData = ueVertBuffer.VertexData;
    const auto& TexcoordData = ueMeshBuffer.TexcoordData;
    const auto& IndexStorage = ueIndexBuffer.IndexStorage;

    glowl::Mesh::VertexPtrDataList data;
    data.push_back({VertexData.data.data(), static_cast<std::size_t>(VertexData.SerializedElementSize * VertexData.Num),
        {12, {{3, GL_FLOAT, GL_FALSE, 0}}}});
    data.push_back(
        {ueMeshBuffer.TangentsData.data.data(), static_cast<std::size_t>(ueMeshBuffer.TangentsData.data.size()),
            {8, {{4, GL_BYTE, GL_TRUE, 0}, {4, GL_BYTE, GL_TRUE, 4}}}});

    std::vector<glowl::VertexLayout::Attribute> texcoordAttributes;
    texcoordAttributes.emplace_back(2, GL_HALF_FLOAT, GL_FALSE, 0);
    if (ueMeshBuffer.NumTexCoords >= 2) {
        texcoordAttributes.emplace_back(2, GL_HALF_FLOAT, GL_FALSE, 4);
    }
    data.push_back(
        {TexcoordData.data.data(), static_cast<std::size_t>(TexcoordData.SerializedElementSize * TexcoordData.Num),
            {static_cast<GLsizei>(ueMeshBuffer.NumTexCoords * 4), texcoordAttributes}});

    return std::make_shared<glowl::Mesh>(data, IndexStorage.data.data(),
        IndexStorage.SerializedElementSize * IndexStorage.Num, GL_UNSIGNED_SHORT);
}
