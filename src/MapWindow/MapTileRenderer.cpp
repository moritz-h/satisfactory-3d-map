#include "MapTileRenderer.h"

#include <iostream>

#include <glm/gtc/matrix_inverse.hpp>

#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapTileRenderer::MapTileRenderer() : show_(true) {
    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

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

    mapTiles_ = {
        {std::make_shared<Model>("map/Tile_X0_Y0_land.glb"), x[0], y[0], true},
        {std::make_shared<Model>("map/Tile_X0_Y0_proxy.glb"), x[0], y[0], false},
        {std::make_shared<Model>("map/Tile_X0_Y1_land.glb"), x[0], y[1], true},
        {std::make_shared<Model>("map/Tile_X0_Y1_proxy.glb"), x[0], y[1], false},
        {std::make_shared<Model>("map/Tile_X0_Y2_land.glb"), x[0], y[2], true},
        {std::make_shared<Model>("map/Tile_X0_Y2_proxy.glb"), x[0], y[2], false},
        {std::make_shared<Model>("map/Tile_X0_Y3_land.glb"), x[0], y[3], true},
        {std::make_shared<Model>("map/Tile_X0_Y3_proxy.glb"), x[0], y[3], false},
        {std::make_shared<Model>("map/Tile_X0_Y4_land.glb"), x[0], y[4], true},
        {std::make_shared<Model>("map/Tile_X0_Y4_proxy.glb"), x[0], y[4], false},
        {std::make_shared<Model>("map/Tile_X0_Y5_land.glb"), x[0], y[5], true},
        {std::make_shared<Model>("map/Tile_X1_Y0_land.glb"), x[1], y[0], true},
        {std::make_shared<Model>("map/Tile_X1_Y0_proxy.glb"), x[1], y[0], false},
        {std::make_shared<Model>("map/Tile_X1_Y1_land.glb"), x[1], y[1], true},
        {std::make_shared<Model>("map/Tile_X1_Y1_proxy.glb"), x[1], y[1], false},
        {std::make_shared<Model>("map/Tile_X1_Y2_land.glb"), x[1], y[2], true},
        {std::make_shared<Model>("map/Tile_X1_Y2_proxy.glb"), x[1], y[2], false},
        {std::make_shared<Model>("map/Tile_X1_Y3_land.glb"), x[1], y[3], true},
        {std::make_shared<Model>("map/Tile_X1_Y3_proxy.glb"), x[1], y[3], false},
        {std::make_shared<Model>("map/Tile_X1_Y4_land.glb"), x[1], y[4], true},
        {std::make_shared<Model>("map/Tile_X1_Y4_proxy.glb"), x[1], y[4], false},
        {std::make_shared<Model>("map/Tile_X1_Y5_land.glb"), x[1], y[5], true},
        {std::make_shared<Model>("map/Tile_X2_Y0_land.glb"), x[2], y[0], true},
        {std::make_shared<Model>("map/Tile_X2_Y0_proxy.glb"), x[2], y[0], false},
        {std::make_shared<Model>("map/Tile_X2_Y1_land.glb"), x[2], y[1], true},
        {std::make_shared<Model>("map/Tile_X2_Y1_proxy.glb"), x[2], y[1], false},
        {std::make_shared<Model>("map/Tile_X2_Y2_land.glb"), x[2], y[2], true},
        {std::make_shared<Model>("map/Tile_X2_Y2_proxy.glb"), x[2], y[2], false},
        {std::make_shared<Model>("map/Tile_X2_Y3_land.glb"), x[2], y[3], true},
        {std::make_shared<Model>("map/Tile_X2_Y3_proxy.glb"), x[2], y[3], false},
        {std::make_shared<Model>("map/Tile_X2_Y4_land.glb"), x[2], y[4], true},
        {std::make_shared<Model>("map/Tile_X2_Y4_proxy.glb"), x[2], y[4], false},
        {std::make_shared<Model>("map/Tile_X2_Y5_land.glb"), x[2], y[5], true},
        {std::make_shared<Model>("map/Tile_X2_Y5_proxy.glb"), x[2], y[5], false},
        {std::make_shared<Model>("map/Tile_X3_Y0_land.glb"), x[3], y[0], true},
        {std::make_shared<Model>("map/Tile_X3_Y0_proxy.glb"), x[3], y[0], false},
        {std::make_shared<Model>("map/Tile_X3_Y1_land.glb"), x[3], y[1], true},
        {std::make_shared<Model>("map/Tile_X3_Y1_proxy.glb"), x[3], y[1], false},
        {std::make_shared<Model>("map/Tile_X3_Y2_land.glb"), x[3], y[2], true},
        {std::make_shared<Model>("map/Tile_X3_Y2_proxy.glb"), x[3], y[2], false},
        {std::make_shared<Model>("map/Tile_X3_Y3_land.glb"), x[3], y[3], true},
        {std::make_shared<Model>("map/Tile_X3_Y3_proxy.glb"), x[3], y[3], false},
        {std::make_shared<Model>("map/Tile_X3_Y4_land.glb"), x[3], y[4], true},
        {std::make_shared<Model>("map/Tile_X3_Y4_proxy.glb"), x[3], y[4], false},
        {std::make_shared<Model>("map/Tile_X3_Y5_land.glb"), x[3], y[5], true},
        {std::make_shared<Model>("map/Tile_X3_Y5_proxy.glb"), x[3], y[5], false},
        {std::make_shared<Model>("map/Tile_X4_Y0_land.glb"), x[4], y[0], true},
        {std::make_shared<Model>("map/Tile_X4_Y0_proxy.glb"), x[4], y[0], false},
        {std::make_shared<Model>("map/Tile_X4_Y1_land.glb"), x[4], y[1], true},
        {std::make_shared<Model>("map/Tile_X4_Y1_proxy.glb"), x[4], y[1], false},
        {std::make_shared<Model>("map/Tile_X4_Y2_land.glb"), x[4], y[2], true},
        {std::make_shared<Model>("map/Tile_X4_Y2_proxy.glb"), x[4], y[2], false},
        {std::make_shared<Model>("map/Tile_X4_Y3_land.glb"), x[4], y[3], true},
        {std::make_shared<Model>("map/Tile_X4_Y3_proxy.glb"), x[4], y[3], false},
        {std::make_shared<Model>("map/Tile_X4_Y4_land.glb"), x[4], y[4], true},
        {std::make_shared<Model>("map/Tile_X4_Y4_proxy.glb"), x[4], y[4], false},
        {std::make_shared<Model>("map/Tile_X4_Y5_land.glb"), x[4], y[5], true},
        {std::make_shared<Model>("map/Tile_X4_Y5_proxy.glb"), x[4], y[5], false},
        {std::make_shared<Model>("map/Tile_X5_Y1_land.glb"), x[5], y[1], true},
        {std::make_shared<Model>("map/Tile_X5_Y1_proxy.glb"), x[5], y[1], false},
        {std::make_shared<Model>("map/Tile_X5_Y2_land.glb"), x[5], y[2], true},
        {std::make_shared<Model>("map/Tile_X5_Y2_proxy.glb"), x[5], y[2], false},
        {std::make_shared<Model>("map/Tile_X5_Y3_land.glb"), x[5], y[3], true},
        {std::make_shared<Model>("map/Tile_X5_Y3_proxy.glb"), x[5], y[3], false},
        {std::make_shared<Model>("map/Tile_X5_Y4_land.glb"), x[5], y[4], true},
        {std::make_shared<Model>("map/Tile_X5_Y4_proxy.glb"), x[5], y[4], false},
        {std::make_shared<Model>("map/Tile_X5_Y5_land.glb"), x[5], y[5], true},
        {std::make_shared<Model>("map/Tile_X5_Y5_proxy.glb"), x[5], y[5], false},
        {std::make_shared<Model>("map/Tile_X6_Y3_land.glb"), x[6], y[3], true},
        {std::make_shared<Model>("map/Tile_X6_Y3_proxy.glb"), x[6], y[3], false},
        {std::make_shared<Model>("map/Tile_X6_Y4_land.glb"), x[6], y[4], true},
        {std::make_shared<Model>("map/Tile_X6_Y4_proxy.glb"), x[6], y[4], false},
        {std::make_shared<Model>("map/Tile_X6_Y5_land.glb"), x[6], y[5], true},
        {std::make_shared<Model>("map/Tile_X6_Y5_proxy.glb"), x[6], y[5], false},
    };
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
