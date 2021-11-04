#ifndef SATISFACTORY3DMAP_MODELMANAGER_H
#define SATISFACTORY3DMAP_MODELMANAGER_H

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Model.h"

namespace Satisfactory3DMap {
    class SaveActor;

    class ModelManager {
    public:
        enum class ModelType {
            None,
            Model,
            SplineModel,
        };

        ModelManager();
        ~ModelManager() = default;

        std::pair<ModelType, int32_t> classifyActor(const Satisfactory3DMap::SaveActor& actor);

        [[nodiscard]] const std::vector<std::unique_ptr<Model>>& models() const {
            return models_;
        };

        [[nodiscard]] const std::vector<std::unique_ptr<Model>>& splineModels() const {
            return splineModels_;
        };

    protected:
        std::vector<std::unique_ptr<Model>> models_;
        std::vector<std::unique_ptr<Model>> splineModels_;

        std::vector<std::vector<std::string>> modelSavePaths_;
        std::vector<std::vector<std::string>> splineModelSavePaths_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MODELMANAGER_H
