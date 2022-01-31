#include "MapTileRenderer.h"

#include <iostream>
#include <regex>

#include <glm/gtc/matrix_inverse.hpp>

#include "GameTypes/Serialization/StaticMesh.h"
#include "Pak/Paks.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapTileRenderer::MapTileRenderer() : wireframe_(false), show_(true) {

    const std::vector<float> x = {
        -254000.0f,
        -152400.0f,
        -50800.0f,
        50800.0f,
        152400.0f,
        254000.0f,
        355600.0f,
    };
    const std::vector<float> y = {
        254000.0f,
        152400.0f,
        50800.0f,
        -50800.0f,
        -152400.0f,
        -254000.0f,
    };

    try {
        PakFile pak = Paks::getMainPakFile();

        const std::regex regex("FactoryGame/Content/FactoryGame/Map/GameLevel01/Tile_X([0-9]+)_Y([0-9]+)LOD/"
                               "SM_(Landscape|PROXY_Tile).*\\.uasset");
        std::smatch match;

        for (const auto& filename : pak.getAllAssetFilenames()) {
            if (std::regex_match(filename, match, regex)) {
                if (match.size() != 4) {
                    throw std::runtime_error("Filename regex error!");
                }
                int tileX = std::stoi(match[1].str());
                int tileY = std::stoi(match[2].str());
                bool offset = match[3].str() == "Landscape";

                AssetFile asset = pak.readAsset(filename);

                // Validate asset has exactly one StaticMesh export
                int staticMeshExportIdx = -1;
                for (int i = 0; i < asset.exportMap().size(); i++) {
                    const auto& exportEntry = asset.exportMap()[i];
                    if (exportEntry.ClassIndex >= 0) {
                        throw std::runtime_error("exportEntry.ClassIndex >= 0 not implemented!");
                    }
                    const auto& importEntry = asset.importMap().at(-exportEntry.ClassIndex - 1);
                    if (importEntry.ObjectName == "StaticMesh") {
                        if (staticMeshExportIdx >= 0) {
                            throw std::runtime_error("Found more than one StaticMesh in asset!");
                        }
                        staticMeshExportIdx = i;
                    }
                }
                if (staticMeshExportIdx < 0) {
                    throw std::runtime_error("No StaticMesh found in asset!");
                }
                const auto& staticMeshExportEntry = asset.exportMap()[staticMeshExportIdx];

                // Serialize StaticMesh
                asset.seek(staticMeshExportEntry.SerialOffset);
                StaticMesh staticMesh;
                asset << staticMesh;

                const auto& ueVertBuffer = staticMesh.renderData().LODResources[0].VertexBuffers.PositionVertexBuffer;
                if (ueVertBuffer.Stride != 12) {
                    throw std::runtime_error("ueVertBuffer.Stride != 12 not implemented!");
                }

                const auto& ueIndexBuffer = staticMesh.renderData().LODResources[0].IndexBuffer;
                if (ueIndexBuffer.b32Bit) {
                    throw std::runtime_error("ueIndexBuffer.b32Bit not implemented!");
                }
                if (ueIndexBuffer.IndexStorage.SerializedElementSize != 1) {
                    throw std::runtime_error("ueIndexBuffer.IndexStorage.SerializedElementSize != 1 not implemented!");
                }

                const auto* ueVertPtr = reinterpret_cast<const glm::vec3*>(ueVertBuffer.VertexData.data.data());
                std::vector<glm::vec3> vertices(ueVertPtr, ueVertPtr + ueVertBuffer.NumVertices);

                const auto* ueIndexPtr = reinterpret_cast<const uint16_t*>(ueIndexBuffer.IndexStorage.data.data());
                std::vector<uint16_t> indices(ueIndexPtr, ueIndexPtr + ueIndexBuffer.IndexStorage.Num / 2);

                {
                    MapTileData mapTile;

                    glGenVertexArrays(1, &mapTile.vao);
                    glBindVertexArray(mapTile.vao);

                    GLuint vbo;
                    glGenBuffers(1, &vbo);
                    glBindBuffer(GL_ARRAY_BUFFER, vbo);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

                    GLuint ibo;
                    glGenBuffers(1, &ibo);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(),
                        GL_STATIC_DRAW);
                    mapTile.indices = ueIndexBuffer.IndexStorage.Num / 2;

                    glBindVertexArray(0);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                    mapTile.x = x[tileX];
                    mapTile.y = y[tileY];
                    mapTile.offset = offset;
                    mapTile.tileX = tileX;
                    mapTile.tileY = tileY;

                    mapTiles_.push_back(mapTile);
                }

                // TODO
            }
        }
    } catch (const std::exception& ex) {
        std::cout << std::string("Error loading Pak file: ") + ex.what() << std::endl;
    }

    try {
        shaderGltf_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_gltf.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_gltf.frag")}});
        shaderMesh_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_mesh.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_mesh.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    const std::vector<MapTileInfo> mapTileInfoList{
        {"map/Tile_X0_Y0_land.glb", 0, 0, true},
        {"map/Tile_X0_Y0_proxy.glb", 0, 0, false},
        {"map/Tile_X0_Y1_land.glb", 0, 1, true},
        {"map/Tile_X0_Y1_proxy.glb", 0, 1, false},
        {"map/Tile_X0_Y2_land.glb", 0, 2, true},
        {"map/Tile_X0_Y2_proxy.glb", 0, 2, false},
        {"map/Tile_X0_Y3_land.glb", 0, 3, true},
        {"map/Tile_X0_Y3_proxy.glb", 0, 3, false},
        {"map/Tile_X0_Y4_land.glb", 0, 4, true},
        {"map/Tile_X0_Y4_proxy.glb", 0, 4, false},
        {"map/Tile_X0_Y5_land.glb", 0, 5, true},
        {"map/Tile_X1_Y0_land.glb", 1, 0, true},
        {"map/Tile_X1_Y0_proxy.glb", 1, 0, false},
        {"map/Tile_X1_Y1_land.glb", 1, 1, true},
        {"map/Tile_X1_Y1_proxy.glb", 1, 1, false},
        {"map/Tile_X1_Y2_land.glb", 1, 2, true},
        {"map/Tile_X1_Y2_proxy.glb", 1, 2, false},
        {"map/Tile_X1_Y3_land.glb", 1, 3, true},
        {"map/Tile_X1_Y3_proxy.glb", 1, 3, false},
        {"map/Tile_X1_Y4_land.glb", 1, 4, true},
        {"map/Tile_X1_Y4_proxy.glb", 1, 4, false},
        {"map/Tile_X1_Y5_land.glb", 1, 5, true},
        {"map/Tile_X2_Y0_land.glb", 2, 0, true},
        {"map/Tile_X2_Y0_proxy.glb", 2, 0, false},
        {"map/Tile_X2_Y1_land.glb", 2, 1, true},
        {"map/Tile_X2_Y1_proxy.glb", 2, 1, false},
        {"map/Tile_X2_Y2_land.glb", 2, 2, true},
        {"map/Tile_X2_Y2_proxy.glb", 2, 2, false},
        {"map/Tile_X2_Y3_land.glb", 2, 3, true},
        {"map/Tile_X2_Y3_proxy.glb", 2, 3, false},
        {"map/Tile_X2_Y4_land.glb", 2, 4, true},
        {"map/Tile_X2_Y4_proxy.glb", 2, 4, false},
        {"map/Tile_X2_Y5_land.glb", 2, 5, true},
        {"map/Tile_X2_Y5_proxy.glb", 2, 5, false},
        {"map/Tile_X3_Y0_land.glb", 3, 0, true},
        {"map/Tile_X3_Y0_proxy.glb", 3, 0, false},
        {"map/Tile_X3_Y1_land.glb", 3, 1, true},
        {"map/Tile_X3_Y1_proxy.glb", 3, 1, false},
        {"map/Tile_X3_Y2_land.glb", 3, 2, true},
        {"map/Tile_X3_Y2_proxy.glb", 3, 2, false},
        {"map/Tile_X3_Y3_land.glb", 3, 3, true},
        {"map/Tile_X3_Y3_proxy.glb", 3, 3, false},
        {"map/Tile_X3_Y4_land.glb", 3, 4, true},
        {"map/Tile_X3_Y4_proxy.glb", 3, 4, false},
        {"map/Tile_X3_Y5_land.glb", 3, 5, true},
        {"map/Tile_X3_Y5_proxy.glb", 3, 5, false},
        {"map/Tile_X4_Y0_land.glb", 4, 0, true},
        {"map/Tile_X4_Y0_proxy.glb", 4, 0, false},
        {"map/Tile_X4_Y1_land.glb", 4, 1, true},
        {"map/Tile_X4_Y1_proxy.glb", 4, 1, false},
        {"map/Tile_X4_Y2_land.glb", 4, 2, true},
        {"map/Tile_X4_Y2_proxy.glb", 4, 2, false},
        {"map/Tile_X4_Y3_land.glb", 4, 3, true},
        {"map/Tile_X4_Y3_proxy.glb", 4, 3, false},
        {"map/Tile_X4_Y4_land.glb", 4, 4, true},
        {"map/Tile_X4_Y4_proxy.glb", 4, 4, false},
        {"map/Tile_X4_Y5_land.glb", 4, 5, true},
        {"map/Tile_X4_Y5_proxy.glb", 4, 5, false},
        {"map/Tile_X5_Y1_land.glb", 5, 1, true},
        {"map/Tile_X5_Y1_proxy.glb", 5, 1, false},
        {"map/Tile_X5_Y2_land.glb", 5, 2, true},
        {"map/Tile_X5_Y2_proxy.glb", 5, 2, false},
        {"map/Tile_X5_Y3_land.glb", 5, 3, true},
        {"map/Tile_X5_Y3_proxy.glb", 5, 3, false},
        {"map/Tile_X5_Y4_land.glb", 5, 4, true},
        {"map/Tile_X5_Y4_proxy.glb", 5, 4, false},
        {"map/Tile_X5_Y5_land.glb", 5, 5, true},
        {"map/Tile_X5_Y5_proxy.glb", 5, 5, false},
        {"map/Tile_X6_Y3_land.glb", 6, 3, true},
        {"map/Tile_X6_Y3_proxy.glb", 6, 3, false},
        {"map/Tile_X6_Y4_land.glb", 6, 4, true},
        {"map/Tile_X6_Y4_proxy.glb", 6, 4, false},
        {"map/Tile_X6_Y5_land.glb", 6, 5, true},
        {"map/Tile_X6_Y5_proxy.glb", 6, 5, false},
    };

    for (const auto& t : mapTileInfoList) {
        if (resourceExists(t.filename)) {
            mapTilesGltf_.emplace_back(
                GltfMapTileData{std::make_shared<Model>(t.filename), x[t.x], y[t.y], t.offset, t.x, t.y});
        } else {
            std::cout << "Warning: Missing map tile resource: " << t.filename << std::endl;
        }
    }
}

void Satisfactory3DMap::MapTileRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    if (wireframe_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    shaderMesh_->use();

    shaderMesh_->setUniform("projMx", projMx);
    shaderMesh_->setUniform("viewMx", viewMx);

    for (const auto& tile : mapTiles_) {
        const float offset = tile.offset ? -50800.0f : 0.0f;

        glm::vec3 position_((tile.x + offset) * 0.01f, -(tile.y + offset) * 0.01f, 0.0f);
        glm::vec4 rotation_(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec3 scale_(0.01f, -0.01f, 0.01f);

        const auto translation = glm::translate(glm::mat4(1.0f), position_);
        const auto rotation = glm::mat4_cast(glm::quat(-rotation_.w, rotation_.x, -rotation_.y, rotation_.z));
        const auto scale = glm::scale(glm::mat4(1.0f), scale_);
        glm::mat4 modelMx = translation * rotation * scale;

        shaderMesh_->setUniform("modelMx", modelMx);

        glBindVertexArray(tile.vao);
        glDrawElementsInstanced(GL_TRIANGLES, tile.indices, GL_UNSIGNED_SHORT, nullptr, 1);
        glBindVertexArray(0);
    }

    glUseProgram(0);

    if (wireframe_) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (!show_) {
        return;
    }
    shaderGltf_->use();

    shaderGltf_->setUniform("projMx", projMx);
    shaderGltf_->setUniform("viewMx", viewMx);
    shaderGltf_->setUniform("invViewMx", glm::inverse(viewMx));

    glActiveTexture(GL_TEXTURE0);
    shaderGltf_->setUniform("tex", 0);

    for (const auto& t : mapTilesGltf_) {
        const float offset = t.offset ? -50800.0f : 0.0f;

        glm::vec3 position_(t.x + offset, t.y + offset, 0.0f);
        glm::vec4 rotation_(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec3 scale_(1.0f, 1.0f, 1.0f);

        const auto translation = glm::translate(glm::mat4(1.0f), position_ * glm::vec3(0.01f, -0.01f, 0.01f));
        const auto rotation = glm::mat4_cast(glm::quat(-rotation_.w, rotation_.x, -rotation_.y, rotation_.z));
        const auto scale = glm::scale(glm::mat4(1.0f), scale_);
        glm::mat4 transform = translation * rotation * scale;

        auto modelMx = t.model->modelMx();

        modelMx = transform * modelMx;

        shaderGltf_->setUniform("modelMx", modelMx);
        shaderGltf_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelMx)));
        shaderGltf_->setUniform("nameX", t.tileX);
        shaderGltf_->setUniform("nameY", t.tileY);
        t.model->bindTexture();
        t.model->draw(1);
    }

    glUseProgram(0);
}
