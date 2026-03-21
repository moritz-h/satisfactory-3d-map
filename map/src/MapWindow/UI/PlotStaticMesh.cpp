#include "PlotStaticMesh.h"

#include <cstring>

Satisfactory3DMap::PlotStaticMesh::PlotStaticMesh(const s::UStaticMesh& staticMesh) {
    const auto& LODResources = staticMesh.RenderData.LODResources[0];

    const auto& ueVertBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
    if (ueVertBuffer.Stride != 12) {
        throw std::runtime_error("Unknown format of StaticMesh data not implemented!");
    }

    const auto& ueIndexBuffer = LODResources.IndexBuffer;
    if (ueIndexBuffer.IndexStorage.SerializedElementSize != 1) {
        throw std::runtime_error("ueIndexBuffer.IndexStorage.SerializedElementSize != 1 not implemented!");
    }

    const auto& VertexData = ueVertBuffer.VertexData;
    const auto& IndexStorage = ueIndexBuffer.IndexStorage;

    vertices_.resize(ueVertBuffer.NumVertices);
    for (std::size_t i = 0; i < ueVertBuffer.NumVertices; i++) {
        const float* v = reinterpret_cast<const float*>(VertexData.data.data() + i * ueVertBuffer.Stride);
        vertices_[i] = {static_cast<double>(v[0]), static_cast<double>(v[1]), static_cast<double>(v[2])};
    }

    const std::size_t indexSize = IndexStorage.SerializedElementSize * IndexStorage.Num;
    if (ueIndexBuffer.b32Bit) {
        indices_.resize(indexSize / sizeof(uint32_t));
        std::memcpy(indices_.data(), IndexStorage.data.data(), indexSize);
    } else {
        const uint16_t* indexPtr = reinterpret_cast<const uint16_t*>(IndexStorage.data.data());
        indices_.resize(indexSize / sizeof(uint16_t));
        for (std::size_t i = 0; i < indices_.size(); i++) {
            indices_[i] = indexPtr[i];
        }
    }

    aabb_ = ImPlot3DBox(vertices_[0], vertices_[0]);
    for (const auto& vert : vertices_) {
        aabb_.Expand(vert);
    }

    const ImPlot3DPoint center = (aabb_.Min + aabb_.Max) * 0.5f;
    const ImPlot3DPoint len = aabb_.Max - aabb_.Min;
    const float lenMaxHalf = std::max(len.x, std::max(len.y, len.z)) * 0.5f + 0.5f;
    cube_.Min = ImPlot3DPoint(center.x - lenMaxHalf, center.y - lenMaxHalf, center.z - lenMaxHalf);
    cube_.Max = ImPlot3DPoint(center.x + lenMaxHalf, center.y + lenMaxHalf, center.z + lenMaxHalf);
}
