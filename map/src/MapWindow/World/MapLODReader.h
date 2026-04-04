#pragma once

#include <memory>

#include "SatisfactorySave/GameTypes/UE/Core/Math/Rotator.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Vector.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Engine/StaticMesh.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Engine/Texture2D.h"
#include "SatisfactorySave/Pak/PakManager.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {
    class MapLODReader {
    public:
        struct MapLODMesh {
            std::shared_ptr<s::UStaticMesh> staticMesh;
            std::shared_ptr<s::UTexture2D> baseColorTexture;
            std::shared_ptr<s::UTexture2D> normalTexture;
            std::string actorLabel;
            std::size_t instanceComponentId;
        };

        struct WaterInstance {
            s::FVector location;
            s::FRotator rotation;
            s::FVector scale;
        };

        explicit MapLODReader(const std::shared_ptr<s::PakManager>& pakManager);
        ~MapLODReader() = default;

        [[nodiscard]] inline const std::vector<MapLODMesh>& meshes() const {
            return meshes_;
        }

        [[nodiscard]] inline const auto& waterPlaneMesh() const {
            return waterPlaneMesh_;
        }

        [[nodiscard]] inline const auto& waterInstances() const {
            return waterInstances_;
        }

    private:
        void readWaterInstance(s::AssetFile& mapAsset, std::size_t i);

        std::vector<MapLODMesh> meshes_;
        std::shared_ptr<s::UStaticMesh> waterPlaneMesh_;
        std::vector<WaterInstance> waterInstances_;
    };
} // namespace Satisfactory3DMap
