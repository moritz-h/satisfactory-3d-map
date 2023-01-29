#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "../OpenGL/GlowlFactory.h"
#include "../OpenGL/GltfModel.h"
#include "GameTypes/Serialization/StaticMesh.h"
#include "GameTypes/Structs/Struct.h"
#include "IO/Pak/PakManager.h"
#include "MeshManager.h"

namespace Satisfactory3DMap {
    class SaveActor;

    class ModelManager {
    public:
        enum class ModelType {
            None,
            PakStaticMesh,
            Model,
            SplineModel,
        };

        struct MeshInfo {
            std::shared_ptr<glowl::Mesh> mesh;
            glm::mat4 transform;
        };

        using MeshModel = std::vector<MeshInfo>;

        explicit ModelManager(std::shared_ptr<PakManager> pakManager);
        ~ModelManager() = default;

        std::pair<ModelType, int32_t> classifyActor(const Satisfactory3DMap::SaveActor& actor);

        [[nodiscard]] const std::vector<MeshModel>& pakModels() const {
            return pakModels_;
        };

        [[nodiscard]] const std::vector<std::unique_ptr<GltfModel>>& models() const {
            return models_;
        };

        [[nodiscard]] const std::vector<std::unique_ptr<GltfModel>>& splineModels() const {
            return splineModels_;
        };

    protected:
        std::optional<int32_t> findPakModel(const std::string& className);
        std::size_t loadAsset(const std::string& className);

        std::shared_ptr<glowl::Mesh> readStaticMeshFromReference(AssetFile& asset,
            const ObjectReference& objectReference);
        MeshInfo getStaticMeshTransformFromStruct(AssetFile& asset, const std::unique_ptr<Struct>& instanceDataStruct);

        std::shared_ptr<PakManager> pakManager_;
        std::shared_ptr<MeshManager> meshManager_;

        std::vector<MeshModel> pakModels_;
        std::unordered_map<std::string, std::size_t> classNameToPakModelMap_;
        std::unordered_set<std::string> classNamesNotInPak_;

        std::vector<std::unique_ptr<GltfModel>> models_;
        std::vector<std::unique_ptr<GltfModel>> splineModels_;

        std::vector<std::vector<std::string>> modelSavePaths_;
        std::vector<std::vector<std::string>> splineModelSavePaths_;
    };
} // namespace Satisfactory3DMap
