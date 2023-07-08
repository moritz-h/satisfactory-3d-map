#include "MapTileRenderer.h"

#include <iostream>
#include <regex>

#include <glm/gtc/matrix_inverse.hpp>
#include <spdlog/spdlog.h>

#include "../OpenGL/Texture.h"
#include "GameTypes/Serialization/StaticMesh.h"
#include "GameTypes/Serialization/Texture2D.h"
#include "Utils/ResourceUtils.h"

namespace {
    const Satisfactory3DMap::ObjectExport& getExportByClass(const Satisfactory3DMap::AssetFile& asset,
        const std::string& class_name) {
        // Validate asset has exactly one "class_name" export
        int exportIdx = -1;
        for (int i = 0; i < asset.exportMap().size(); i++) {
            const auto& exportEntry = asset.exportMap()[i];
            if (exportEntry.ClassIndex >= 0) {
                throw std::runtime_error("exportEntry.ClassIndex >= 0 not implemented!");
            }
            const auto& importEntry = asset.importMap().at(-exportEntry.ClassIndex - 1);
            if (importEntry.ObjectName == class_name) {
                if (exportIdx >= 0) {
                    throw std::runtime_error("Found more than one " + class_name + " in asset!");
                }
                exportIdx = i;
            }
        }
        if (exportIdx < 0) {
            throw std::runtime_error("No " + class_name + " found in asset!");
        }
        return asset.exportMap()[exportIdx];
    }
} // namespace

Satisfactory3DMap::MapTileRenderer::MapTileRenderer(const std::shared_ptr<Configuration>& config,
    const std::shared_ptr<PakManager>& pakManager) {

    wireframeSetting_ = BoolSetting::create("Tile wireframe", false);
    config->registerSetting(wireframeSetting_);

    faceNormalsSetting_ = BoolSetting::create("Use face normals", false);
    config->registerSetting(faceNormalsSetting_);

    const std::vector<float> pos_x = {
        -254000.0f,
        -152400.0f,
        -50800.0f,
        50800.0f,
        152400.0f,
        254000.0f,
        355600.0f,
    };
    const std::vector<float> pos_y = {
        254000.0f,
        152400.0f,
        50800.0f,
        -50800.0f,
        -152400.0f,
        -254000.0f,
    };

    if (pakManager != nullptr) {
        try {

            const std::regex regex(
                "Game/FactoryGame/Map/GameLevel01/Tile_X([0-9]+)_Y([0-9]+)LOD/SM_(Landscape|PROXY_Tile).*\\.uasset");
            std::smatch match;

            for (const auto& filename : pakManager->getAllAssetFilenames()) {
                if (std::regex_match(filename, match, regex)) {
                    if (match.size() != 4) {
                        throw std::runtime_error("Filename regex error!");
                    }
                    int tileX = std::stoi(match[1].str());
                    int tileY = std::stoi(match[2].str());
                    bool isLandscape = match[3].str() == "Landscape";
                    bool isNewPosFormat = tileX > 4 || (tileX > 1 && tileY > 3); // Changed tiles with Update 6.

                    AssetFile asset = pakManager->readAsset(filename);

                    const auto& staticMeshExportEntry = getExportByClass(asset, "StaticMesh");

                    // Serialize StaticMesh
                    asset.seek(staticMeshExportEntry.SerialOffset);
                    StaticMesh staticMesh;
                    asset << staticMesh;

                    // Textures
                    std::string texname = std::regex_replace(filename, std::regex("SM_"), "T_");
                    std::string texname_d;
                    std::string texname_n;
                    if (isLandscape) {
                        texname_d = std::regex_replace(texname, std::regex(".uasset"), "_D.uasset");
                        texname_n = std::regex_replace(texname, std::regex(".uasset"), "_N.uasset");
                    } else {
                        texname_d = std::regex_replace(texname, std::regex(".uasset"), "_Diffuse.uasset");
                        texname_n = std::regex_replace(texname, std::regex(".uasset"), "_Normal.uasset");
                    }
                    if (!pakManager->containsAssetFilename(texname_d) ||
                        !pakManager->containsAssetFilename(texname_n)) {
                        throw std::runtime_error("Texture not found!");
                    }

                    // Diffuse texture
                    AssetFile assetTexD = pakManager->readAsset(texname_d);
                    const auto& texDExportEntry = getExportByClass(assetTexD, "Texture2D");

                    assetTexD.seek(texDExportEntry.SerialOffset);
                    Texture2D texD;
                    assetTexD << texD;

                    // Normal texture
                    AssetFile assetTexN = pakManager->readAsset(texname_n);
                    const auto& texNExportEntry = getExportByClass(assetTexN, "Texture2D");

                    assetTexN.seek(texNExportEntry.SerialOffset);
                    Texture2D texN;
                    assetTexN << texN;

                    // Render data
                    MapTileData mapTile;
                    mapTile.mesh = makeGlowlMesh(staticMesh);
                    mapTile.texD = makeOpenGLTexture(texD);
                    mapTile.texN = makeOpenGLTexture(texN);
                    mapTile.tileX = tileX;
                    mapTile.tileY = tileY;

                    // Precalculate matrices
                    float x = 0.0f;
                    float y = 0.0f;
                    if (isLandscape || !isNewPosFormat) {
                        x = pos_x[tileX];
                        y = pos_y[tileY];
                    }

                    const float offset = isLandscape ? -50800.0f : 0.0f;

                    glm::vec3 position_((x + offset) * 0.01f, -(y + offset) * 0.01f, 0.0f);
                    glm::vec4 rotation_(0.0f, 0.0f, 0.0f, 1.0f);
                    glm::vec3 scale_(0.01f, -0.01f, 0.01f);

                    const auto translation = glm::translate(glm::mat4(1.0f), position_);
                    const auto rotation = glm::mat4_cast(glm::quat(-rotation_.w, rotation_.x, -rotation_.y, rotation_.z));
                    const auto scale = glm::scale(glm::mat4(1.0f), scale_);

                    mapTile.modelMx = translation * rotation * scale;
                    mapTile.normalMx = glm::inverseTranspose(glm::mat3(mapTile.modelMx));

                    mapTiles_.push_back(std::move(mapTile));
                }
            }
        } catch (const std::exception& ex) {
            spdlog::warn("Error loading Pak file: {}", ex.what());
        }
    }

    try {
        meshShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_mesh.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_mesh.frag")}});

        flatShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_flat.vert")},
            {glowl::GLSLProgram::ShaderType::Geometry, getStringResource("shaders/maptile_flat.geom")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_flat.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }
}

void Satisfactory3DMap::MapTileRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    if (wireframeSetting_->getVal()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    const std::unique_ptr<glowl::GLSLProgram>& shader = faceNormalsSetting_->getVal() ? flatShader_ : meshShader_;

    shader->use();

    shader->setUniform("projMx", projMx);
    shader->setUniform("viewMx", viewMx);

    for (const auto& tile : mapTiles_) {
        shader->setUniform("modelMx", tile.modelMx);
        shader->setUniform("normalMx", tile.normalMx);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tile.texD);
        shader->setUniform("texD", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tile.texN);
        shader->setUniform("texN", 1);

        tile.mesh->draw();
    }

    glUseProgram(0);

    if (wireframeSetting_->getVal()) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
