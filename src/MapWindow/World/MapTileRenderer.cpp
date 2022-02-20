#include "MapTileRenderer.h"

#include <iostream>
#include <regex>

#include <glm/gtc/matrix_inverse.hpp>

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

Satisfactory3DMap::MapTileRenderer::MapTileRenderer(const std::shared_ptr<PakFile>& pak)
    : wireframe_(false),
      show_(true) {

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

    if (pak != nullptr) {
        try {

            const std::regex regex("FactoryGame/Content/FactoryGame/Map/GameLevel01/Tile_X([0-9]+)_Y([0-9]+)LOD/"
                                   "SM_(Landscape|PROXY_Tile).*\\.uasset");
            std::smatch match;

            for (const auto& filename : pak->getAllAssetFilenames()) {
                if (std::regex_match(filename, match, regex)) {
                    if (match.size() != 4) {
                        throw std::runtime_error("Filename regex error!");
                    }
                    int tileX = std::stoi(match[1].str());
                    int tileY = std::stoi(match[2].str());
                    bool isLandscape = match[3].str() == "Landscape";

                    AssetFile asset = pak->readAsset(filename);

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
                    if (!pak->containsAssetFilename(texname_d) || !pak->containsAssetFilename(texname_n)) {
                        throw std::runtime_error("Texture not found!");
                    }

                    // Diffuse texture
                    AssetFile assetTexD = pak->readAsset(texname_d);
                    const auto& texDExportEntry = getExportByClass(assetTexD, "Texture2D");

                    assetTexD.seek(texDExportEntry.SerialOffset);
                    Texture2D texD;
                    assetTexD << texD;

                    // Normal texture
                    AssetFile assetTexN = pak->readAsset(texname_n);
                    const auto& texNExportEntry = getExportByClass(assetTexN, "Texture2D");

                    assetTexN.seek(texNExportEntry.SerialOffset);
                    Texture2D texN;
                    assetTexN << texN;

                    // Render data
                    MapTileData mapTile;
                    mapTile.mesh = std::make_unique<StaticMeshVAO>(staticMesh);
                    mapTile.texD = makeOpenGLTexture(texD);
                    mapTile.texN = makeOpenGLTexture(texN);

                    mapTile.x = x[tileX];
                    mapTile.y = y[tileY];
                    mapTile.offset = isLandscape;
                    mapTile.tileX = tileX;
                    mapTile.tileY = tileY;

                    mapTiles_.push_back(std::move(mapTile));
                }
            }
        } catch (const std::exception& ex) {
            std::cout << std::string("Error loading Pak file: ") + ex.what() << std::endl;
        }
    }

    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_mesh.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_mesh.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }
}

void Satisfactory3DMap::MapTileRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    if (!show_) {
        return;
    }

    if (wireframe_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    shader_->use();

    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);

    for (const auto& tile : mapTiles_) {
        const float offset = tile.offset ? -50800.0f : 0.0f;

        glm::vec3 position_((tile.x + offset) * 0.01f, -(tile.y + offset) * 0.01f, 0.0f);
        glm::vec4 rotation_(0.0f, 0.0f, 0.0f, 1.0f);
        glm::vec3 scale_(0.01f, -0.01f, 0.01f);

        const auto translation = glm::translate(glm::mat4(1.0f), position_);
        const auto rotation = glm::mat4_cast(glm::quat(-rotation_.w, rotation_.x, -rotation_.y, rotation_.z));
        const auto scale = glm::scale(glm::mat4(1.0f), scale_);
        glm::mat4 modelMx = translation * rotation * scale;

        shader_->setUniform("modelMx", modelMx);
        shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelMx)));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tile.texD);
        shader_->setUniform("texD", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tile.texN);
        shader_->setUniform("texN", 1);

        tile.mesh->draw();
    }

    glUseProgram(0);

    if (wireframe_) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
