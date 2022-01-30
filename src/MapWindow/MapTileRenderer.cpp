#include "MapTileRenderer.h"

#include <iostream>
#include <regex>

#include <glm/gtc/matrix_inverse.hpp>

#include "GameTypes/Serialization/StaticMesh.h"
#include "Pak/Paks.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapTileRenderer::MapTileRenderer() : show_(true) {

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

                // TODO
            }
        }
    } catch (const std::exception& ex) {
        std::cout << std::string("Error loading Pak file: ") + ex.what() << std::endl;
    }

    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile.frag")}});
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

    for (const auto& t : mapTileInfoList) {
        if (resourceExists(t.filename)) {
            mapTiles_.emplace_back(MapTileData{std::make_shared<Model>(t.filename), x[t.x], y[t.y], t.offset});
        } else {
            std::cout << "Warning: Missing map tile resource: " << t.filename << std::endl;
        }
    }
}

void Satisfactory3DMap::MapTileRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    if (!show_) {
        return;
    }
    shader_->use();

    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);
    shader_->setUniform("invViewMx", glm::inverse(viewMx));

    glActiveTexture(GL_TEXTURE0);
    shader_->setUniform("tex", 0);

    for (const auto& t : mapTiles_) {
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

        shader_->setUniform("modelMx", modelMx);
        shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelMx)));
        t.model->bindTexture();
        t.model->draw(1);
    }

    glUseProgram(0);
}
