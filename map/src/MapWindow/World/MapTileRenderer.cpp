#include "MapTileRenderer.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <spdlog/spdlog.h>

#include "../OpenGL/GlowlFactory.h"
#include "MapLODReader.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapTileRenderer::MapTileRenderer(const std::shared_ptr<Configuration>& config,
    const std::shared_ptr<SatisfactorySave::PakManager>& pakManager) {

    wireframeSetting_ = BoolSetting::create("Tile wireframe", false);
    config->registerSetting(wireframeSetting_);

    faceNormalsSetting_ = BoolSetting::create("Use face normals", false);
    config->registerSetting(faceNormalsSetting_);

    MapLODReader LODReader(pakManager);
    for (const auto& mesh : LODReader.meshes()) {
        MapTileData mapTile;
        mapTile.mesh = makeGlowlMesh(*mesh.staticMesh);

        mapTile.modelMx = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
        mapTile.normalMx = glm::inverseTranspose(glm::mat3(mapTile.modelMx));

        mapTile.texBaseColor = std::make_unique<OGLTexture2D>(*mesh.baseColorTexture);
        mapTile.texNormal = std::make_unique<OGLTexture2D>(*mesh.normalTexture);

        mapTiles_.push_back(std::move(mapTile));
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
        tile.texBaseColor->bindTexture();
        shader->setUniform("texBaseColor", 0);

        glActiveTexture(GL_TEXTURE1);
        tile.texNormal->bindTexture();
        shader->setUniform("texNormal", 1);

        tile.mesh->draw();
    }

    glUseProgram(0);

    if (wireframeSetting_->getVal()) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
