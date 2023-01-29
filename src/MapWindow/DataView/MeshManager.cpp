#include "MeshManager.h"

#include "../OpenGL/GlowlFactory.h"
#include "GameTypes/Serialization/StaticMesh.h"
#include "Utils/StringUtils.h"

Satisfactory3DMap::MeshManager::MeshManager(std::shared_ptr<PakManager> pakManager)
    : pakManager_(std::move(pakManager)) {}

std::shared_ptr<glowl::Mesh> Satisfactory3DMap::MeshManager::loadMesh(const std::string& className) {
    auto meshIt = meshes_.find(className);
    if (meshIt != meshes_.end()) {
        if (meshIt->second == nullptr) {
            throw std::runtime_error("Mesh class name not found: " + className);
        }
        return meshIt->second;
    }

    try {
        auto [assetName, objectName] = splitOnChar(className, '.');
        assetName = PakManager::classNameToAssetPath(assetName);

        if (!pakManager_->containsAssetFilename(assetName)) {
            throw std::runtime_error("Asset missing: " + assetName);
        }

        auto StaticMeshAsset = pakManager_->readAsset(assetName);

        StaticMeshAsset.seek(StaticMeshAsset.getObjectExportEntry(objectName).SerialOffset);

        StaticMesh mesh;
        StaticMeshAsset << mesh;

        auto gpuMesh = makeGlowlMesh(mesh);

        meshes_.insert({className, gpuMesh});
        return gpuMesh;
    } catch ([[maybe_unused]] std::exception const& ex) {
        // Use null-pointer to store already checked class names
        meshes_.insert({className, nullptr});
        throw;
    }
}
