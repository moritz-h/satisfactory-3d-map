#include "MeshManager.h"

#include "SatisfactorySave/GameTypes/UE/Engine/Engine/StaticMesh.h"
#include "SatisfactorySave/Utils/StringUtils.h"

#include "../OpenGL/GlowlFactory.h"

Satisfactory3DMap::MeshManager::MeshManager(std::shared_ptr<SatisfactorySave::PakManager> pakManager)
    : pakManager_(std::move(pakManager)) {}

std::shared_ptr<glowl::Mesh> Satisfactory3DMap::MeshManager::loadMesh(std::string const& packageName,
    uint64_t publicExportHash) {
    const std::string cache_key = packageName + "." + std::to_string(publicExportHash);
    auto meshIt = meshes_.find(cache_key);
    if (meshIt != meshes_.end()) {
        if (meshIt->second == nullptr) {
            throw std::runtime_error("Mesh class name not found: " + cache_key);
        }
        return meshIt->second;
    }

    try {
        const auto assetName = packageName.substr(1) + ".uasset";

        if (!pakManager_->containsAssetFilename(assetName)) {
            throw std::runtime_error("Asset missing: " + assetName);
        }

        auto StaticMeshAsset = pakManager_->readAsset(assetName);

        StaticMeshAsset.seekCookedSerialOffset(
            StaticMeshAsset.getObjectExportEntry(publicExportHash).CookedSerialOffset);

        SatisfactorySave::UStaticMesh mesh;
        StaticMeshAsset << mesh;

        auto gpuMesh = makeGlowlMesh(mesh);

        meshes_.insert({cache_key, gpuMesh});
        return gpuMesh;
    } catch ([[maybe_unused]] std::exception const& ex) {
        // Use null-pointer to store already checked class names
        meshes_.insert({cache_key, nullptr});
        throw;
    }
}
