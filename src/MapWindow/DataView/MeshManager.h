#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <glowl/glowl.h>

#include "IO/Pak/PakManager.h"

namespace Satisfactory3DMap {
    class MeshManager {
    public:
        explicit MeshManager(std::shared_ptr<PakManager> pakManager);
        ~MeshManager() = default;

        std::shared_ptr<glowl::Mesh> loadMesh(std::string const& className);

    protected:
        std::shared_ptr<PakManager> pakManager_;

        std::unordered_map<std::string, std::shared_ptr<glowl::Mesh>> meshes_;
    };
} // namespace Satisfactory3DMap
