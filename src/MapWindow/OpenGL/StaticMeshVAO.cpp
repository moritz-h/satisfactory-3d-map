#include "StaticMeshVAO.h"

Satisfactory3DMap::StaticMeshVAO::StaticMeshVAO(const Satisfactory3DMap::StaticMesh& mesh) {

    const auto& vertexBuffers = mesh.renderData().LODResources[0].VertexBuffers;
    const auto& ueVertBuffer = vertexBuffers.PositionVertexBuffer;
    const auto& ueMeshBuffer = vertexBuffers.StaticMeshVertexBuffer;
    if (ueVertBuffer.Stride != 12 || ueMeshBuffer.NumVertices != ueVertBuffer.NumVertices ||
        ueMeshBuffer.NumTexCoords != 2 || ueMeshBuffer.TexcoordData.SerializedElementSize != 4) {
        throw std::runtime_error("Unknown format of StaticMesh data not implemented!");
    }

    const auto& ueIndexBuffer = mesh.renderData().LODResources[0].IndexBuffer;
    if (ueIndexBuffer.b32Bit) {
        throw std::runtime_error("ueIndexBuffer.b32Bit not implemented!");
    }
    if (ueIndexBuffer.IndexStorage.SerializedElementSize != 1) {
        throw std::runtime_error("ueIndexBuffer.IndexStorage.SerializedElementSize != 1 not implemented!");
    }

    const auto* ueVertPtr = reinterpret_cast<const glm::vec3*>(ueVertBuffer.VertexData.data.data());
    std::vector<glm::vec3> vertices(ueVertPtr, ueVertPtr + ueVertBuffer.NumVertices);
    const auto* ueTexCoordsPtr = reinterpret_cast<const uint16_t*>(ueMeshBuffer.TexcoordData.data.data());
    std::vector<uint16_t> texCoords(ueTexCoordsPtr,
        ueTexCoordsPtr + ueMeshBuffer.NumVertices * ueMeshBuffer.NumTexCoords * sizeof(uint16_t));

    const auto* ueIndexPtr = reinterpret_cast<const uint16_t*>(ueIndexBuffer.IndexStorage.data.data());
    std::vector<uint16_t> indices(ueIndexPtr, ueIndexPtr + ueIndexBuffer.IndexStorage.Num / 2);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    GLuint vbo[3];
    glGenBuffers(3, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, ueMeshBuffer.TangentsData.data.size(), ueMeshBuffer.TangentsData.data.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_BYTE, GL_TRUE, 8, (void*) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_BYTE, GL_TRUE, 8, (void*) 4);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uint16_t) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_HALF_FLOAT, GL_FALSE, 8, (void*) 0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_HALF_FLOAT, GL_FALSE, 8, (void*) 4);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
    indices_ = ueIndexBuffer.IndexStorage.Num / 2;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Satisfactory3DMap::StaticMeshVAO::draw(std::size_t instanceCount) {
    glBindVertexArray(vao_);
    glDrawElementsInstanced(GL_TRIANGLES, indices_, GL_UNSIGNED_SHORT, nullptr, instanceCount);
    glBindVertexArray(0);
}
