#include "MeshManager.h"

#include "Utils/StringUtils.h"

Satisfactory3DMap::MeshManager::MeshManager(std::shared_ptr<PakManager> pakManager)
    : pakManager_(std::move(pakManager)) {}

std::shared_ptr<Satisfactory3DMap::StaticMesh> Satisfactory3DMap::MeshManager::loadMesh(const std::string& className) {
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

        // TODO remove hardcoded [2] and use objectName
        StaticMeshAsset.seek(StaticMeshAsset.exportMap()[2].SerialOffset);

        auto mesh = std::make_shared<StaticMesh>();
        StaticMeshAsset << *mesh;

        meshes_.insert({className, mesh});
        return mesh;
    } catch ([[maybe_unused]] std::exception const& ex) {
        // Use null-pointer to store already checked class names
        meshes_.insert({className, nullptr});
        throw;
    }
}
