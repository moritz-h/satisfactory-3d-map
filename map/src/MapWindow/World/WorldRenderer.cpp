#include "WorldRenderer.h"

#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/UE/Engine/Engine/Texture2D.h"

#include "../OpenGL/Texture.h"
#include "Utils/ResourceUtils.h"

namespace {
    std::unique_ptr<glowl::Texture2D> makeMapTex(const std::string& resource) {
        int width = 0;
        int height = 0;
        std::vector<unsigned char> mapImage = Satisfactory3DMap::getImageRGBAResource(resource, width, height);
        glowl::TextureLayout texLayout(GL_SRGB8_ALPHA8, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1,
            {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
                {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}},
            {});
        return std::make_unique<glowl::Texture2D>(resource, texLayout, mapImage.data(), true);
    }

    GLuint makeMapTex(const std::shared_ptr<s::PakManager>& pakManager, const std::string& name) {
        auto asset = pakManager->readAsset(name);
        auto exportEntry = asset.exportMap()[0];
        asset.seekCookedSerialOffset(exportEntry.CookedSerialOffset);
        asset.pushReadLimit(exportEntry.CookedSerialSize);
        s::UTexture2D tex;
        asset << tex;
        return Satisfactory3DMap::makeOpenGLTexture(tex);
    }
} // namespace

Satisfactory3DMap::WorldRenderer::WorldRenderer(const std::shared_ptr<Configuration>& config,
    const std::shared_ptr<s::PakManager>& pakManager)
    : vaEmpty_(0),
      texHeightWidth_(0),
      texHeightHeight_(0),
      tessLevelInner_(1),
      tessLevelOuter_(1),
      numInstancesX_(16),
      numInstancesY_(16) {

    useWorldTexSetting_ = BoolSetting::create("Use world tex", true);
    wireframeSetting_ = BoolSetting::create("World wireframe", false);

    config->registerSetting(useWorldTexSetting_);
    config->registerSetting(wireframeSetting_);

    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/world.vert")},
            {glowl::GLSLProgram::ShaderType::TessControl, getStringResource("shaders/world.tesc")},
            {glowl::GLSLProgram::ShaderType::TessEvaluation, getStringResource("shaders/world.tese")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/world.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }

    glGenVertexArrays(1, &vaEmpty_);
    // Bind once to create
    glBindVertexArray(vaEmpty_);
    glBindVertexArray(0);

    if (pakManager == nullptr) {
        std::vector<unsigned char> heightImage =
            getImageRGBAResource("textures/Map/HeightData_Test.png", texHeightWidth_, texHeightHeight_);
        glowl::TextureLayout layout(GL_RGBA8, texHeightWidth_, texHeightHeight_, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1,
            {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
                {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}},
            {});
        texHeight_ = std::make_unique<glowl::Texture2D>("height", layout, heightImage.data());

        texMap00_ = makeMapTex("textures/Map/Map_0-0.png");
        texMap01_ = makeMapTex("textures/Map/Map_0-1.png");
        texMap10_ = makeMapTex("textures/Map/Map_1-0.png");
        texMap11_ = makeMapTex("textures/Map/Map_1-1.png");
    } else {
        texHeightWidth_ = 2048;
        texHeightHeight_ = 2048;
        pakTexHeight_ = makeMapTex(pakManager, "Game/FactoryGame/Interface/UI/Assets/MapTest/HeightData_Test.uasset");
        pakTexMap00_ = makeMapTex(pakManager, "Game/FactoryGame/Interface/UI/Assets/MapTest/SlicedMap/Map_0-0.uasset");
        pakTexMap01_ = makeMapTex(pakManager, "Game/FactoryGame/Interface/UI/Assets/MapTest/SlicedMap/Map_0-1.uasset");
        pakTexMap10_ = makeMapTex(pakManager, "Game/FactoryGame/Interface/UI/Assets/MapTest/SlicedMap/Map_1-0.uasset");
        pakTexMap11_ = makeMapTex(pakManager, "Game/FactoryGame/Interface/UI/Assets/MapTest/SlicedMap/Map_1-1.uasset");
    }

    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &tessLevelInner_);
    tessLevelOuter_ = tessLevelInner_;

    // Integer division on purpose. NOPT values will only lead to a grid coarser than each pixel.
    numInstancesX_ = texHeightWidth_ / tessLevelInner_;
    numInstancesY_ = texHeightHeight_ / tessLevelInner_;
}

void Satisfactory3DMap::WorldRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    if (wireframeSetting_->getVal()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    shader_->use();

    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);
    shader_->setUniform("invViewMx", glm::inverse(viewMx));
    shader_->setUniform("tessLevelInner", tessLevelInner_);
    shader_->setUniform("tessLevelOuter", tessLevelOuter_);
    shader_->setUniform("numInstancesX", numInstancesX_);
    shader_->setUniform("numInstancesY", numInstancesY_);
    shader_->setUniform("useWorldTex", static_cast<int>(useWorldTexSetting_->getVal()));

    // Transfer function x,y,z = A * (u,v,h) + B,
    // where u,v is tex coords in range [0,1] and h is value from height texture in range [0,1].
    // Result x,y,z are global world coords of the map in meter.
    //
    // Height calculation:
    // A x + B = y
    // A = (y1 - y2) / (x1 - x2)
    // B = (y1 * x2 - y2 * x1) / (x2 - x1)
    // Sea level:           0x81 => 129/255 => -17m
    // Desert high plateau: 0xC2 => 194/255 => 235m
    static constexpr float A = (-17.0f - 235.0f) * 255.0f / (129.0f - 194.0f);
    static constexpr float B = (-17.0f * 194.0f - 235.0f * 129.0f) / (194.0f - 129.0f);
    shader_->setUniform("posTransferFuncA", 7500.0f, 7500.0f, A);
    shader_->setUniform("posTransferFuncB", -3246.5f, -3750.0f, B);
    shader_->setUniform("sizeTexHeight", 2048.0f, 2048.0f);

    glActiveTexture(GL_TEXTURE0);
    if (texHeight_ != nullptr) {
        texHeight_->bindTexture();
    } else {
        glBindTexture(GL_TEXTURE_2D, pakTexHeight_);
    }
    shader_->setUniform("texHeight", 0);

    glActiveTexture(GL_TEXTURE1);
    if (texMap00_ != nullptr) {
        texMap00_->bindTexture();
    } else {
        glBindTexture(GL_TEXTURE_2D, pakTexMap00_);
    }
    shader_->setUniform("texMap00", 1);

    glActiveTexture(GL_TEXTURE2);
    if (texMap01_ != nullptr) {
        texMap01_->bindTexture();
    } else {
        glBindTexture(GL_TEXTURE_2D, pakTexMap01_);
    }
    shader_->setUniform("texMap01", 2);

    glActiveTexture(GL_TEXTURE3);
    if (texMap10_ != nullptr) {
        texMap10_->bindTexture();
    } else {
        glBindTexture(GL_TEXTURE_2D, pakTexMap10_);
    }
    shader_->setUniform("texMap10", 3);

    glActiveTexture(GL_TEXTURE4);
    if (texMap00_ != nullptr) {
        texMap11_->bindTexture();
    } else {
        glBindTexture(GL_TEXTURE_2D, pakTexMap11_);
    }
    shader_->setUniform("texMap11", 4);

    glBindVertexArray(vaEmpty_);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawArraysInstanced(GL_PATCHES, 0, 4, numInstancesX_ * numInstancesY_);
    glBindVertexArray(0);

    glUseProgram(0);

    if (wireframeSetting_->getVal()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
