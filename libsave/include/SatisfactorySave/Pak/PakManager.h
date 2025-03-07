#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "PakFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PakManager {
    public:
        static inline std::string classNameToAssetPath(const std::string& className) {
            // Remove leading /
            std::string assetName = className;
            if (!assetName.empty() && assetName[0] == '/') {
                assetName = assetName.substr(1);
            }
            // Remove object name
            const auto idx = assetName.find_last_of('.');
            if (idx != std::string::npos) {
                assetName = assetName.substr(0, idx);
            }
            return assetName + ".uasset";
        }

        static inline std::string classNameToObjectName(const std::string& className) {
            const auto idx = className.find_last_of('.');
            if (idx == std::string::npos) {
                return "";
            }
            return className.substr(idx + 1);
        }

        explicit PakManager(const std::filesystem::path& gameDir);
        ~PakManager() = default;

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const;

        [[nodiscard]] inline bool containsAssetFilename(const std::string& filename) const {
            return packageNames_.contains(filename);
        }

        inline AssetFile readAsset(const std::string& filename) {
            const auto& [pakIdx, pakFilename] = packageNames_.at(filename);
            return pakFiles_.at(pakIdx)->readAsset(pakFilename);
        }

        inline std::vector<char> readAssetFileContent(const std::string& filename) {
            const auto& [pakIdx, pakFilename] = packageNames_.at(filename);
            return pakFiles_.at(pakIdx)->readAssetFileContent(pakFilename);
        }

    protected:
        void cacheLatestPakNames(const std::optional<std::string>& modPrefix = std::nullopt);

        std::vector<std::shared_ptr<PakFile>> pakFiles_;

        std::unordered_map<std::string, std::pair<std::size_t, std::string>> packageNames_;
    };
} // namespace SatisfactorySave
