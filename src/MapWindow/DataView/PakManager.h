#ifndef SATISFACTORY3DMAP_PAKMANAGER_H
#define SATISFACTORY3DMAP_PAKMANAGER_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "IO/Pak/PakFile.h"

namespace Satisfactory3DMap {

    class PakManager {
    public:
        static inline std::string classNameToAssetPath(const std::string& className) {
            // Replace beginning: "/Game" => "FactoryGame/Content"
            std::string assetName = "FactoryGame/Content" + className.substr(5);
            // Remove object name
            assetName = assetName.substr(0, assetName.find_last_of('.')) + ".uasset";
            return assetName;
        }

        explicit PakManager(const std::filesystem::path& gameDir);
        ~PakManager() = default;

        [[nodiscard]] const std::vector<std::shared_ptr<PakFile>>& pakFiles() const {
            return pakFiles_;
        }

        [[nodiscard]] const std::vector<std::string>& pakFileNames() const {
            return pakFileNames_;
        }

        // TODO temporary fix compilation by just mapping to the first pak file.
        std::vector<std::string> getAllAssetFilenames() const { return pakFiles_[0]->getAllAssetFilenames(); };
        bool containsAssetFilename(const std::string& filename) { return pakFiles_[0]->containsAssetFilename(filename); };
        AssetFile readAsset(const std::string& filename) { return pakFiles_[0]->readAsset(filename); };

    protected:
        std::vector<std::shared_ptr<PakFile>> pakFiles_;
        std::vector<std::string> pakFileNames_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PAKMANAGER_H
