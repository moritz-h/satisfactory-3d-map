#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "GameTypes/Serialization/StaticMesh.h"
#include "IO/Pak/PakManager.h"

namespace Satisfactory3DMap {
    class MeshManager {
    public:
        explicit MeshManager(std::shared_ptr<PakManager> pakManager);
        ~MeshManager() = default;

        std::shared_ptr<StaticMesh> loadMesh(std::string const& className);

    protected:
        std::shared_ptr<PakManager> pakManager_;

        std::unordered_map<std::string, std::shared_ptr<StaticMesh>> meshes_;
    };
} // namespace Satisfactory3DMap
