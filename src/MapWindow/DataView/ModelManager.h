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

        explicit ModelManager(std::shared_ptr<PakManager> pakManager);
        ~ModelManager() = default;

        std::pair<ModelType, int32_t> classifyActor(const Satisfactory3DMap::SaveActor& actor);

        [[nodiscard]] const std::vector<std::shared_ptr<glowl::Mesh>>& pakModels() const {
            return pakModels_;
        };

        [[nodiscard]] const std::vector<glm::mat4>& pakTransformations() const {
            return pakTransformations_;
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

        std::shared_ptr<StaticMesh> readStaticMeshFromReference(AssetFile& asset,
            const ObjectReference& objectReference);
        std::tuple<std::shared_ptr<StaticMesh>, glm::mat4> getStaticMeshTransformFromStruct(AssetFile& asset,
            const std::unique_ptr<Struct>& instanceDataStruct);

        std::shared_ptr<PakManager> pakManager_;
        std::shared_ptr<MeshManager> meshManager_;

        std::vector<std::shared_ptr<glowl::Mesh>> pakModels_;
        std::vector<glm::mat4> pakTransformations_;
        std::unordered_map<std::string, std::size_t> classNameToPakModelMap_;
        std::unordered_set<std::string> classNamesNotInPak_;

        std::vector<std::unique_ptr<GltfModel>> models_;
        std::vector<std::unique_ptr<GltfModel>> splineModels_;

        std::vector<std::vector<std::string>> modelSavePaths_;
        std::vector<std::vector<std::string>> splineModelSavePaths_;
    };
} // namespace Satisfactory3DMap
