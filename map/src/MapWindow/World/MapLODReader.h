#pragma once

#include <memory>

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

        explicit MapLODReader(const std::shared_ptr<SatisfactorySave::PakManager>& pakManager);
        ~MapLODReader() = default;

        [[nodiscard]] const std::vector<MapLODMesh>& meshes() const {
            return meshes_;
        }

    private:
        std::vector<MapLODMesh> meshes_;
    };
} // namespace Satisfactory3DMap
