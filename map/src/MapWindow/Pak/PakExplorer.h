#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "../DataView/DataView.h"
#include "AssetWindow.h"

namespace Satisfactory3DMap {

    class PakExplorer : public std::enable_shared_from_this<PakExplorer> {
    public:
        explicit PakExplorer(std::shared_ptr<DataView> dataView);
        ~PakExplorer() = default;

        void renderGui();

        bool& show() {
            return show_;
        }

        [[nodiscard]] const std::shared_ptr<SatisfactorySave::PakManager>& pakManager() const {
            return dataView_->pakManager();
        }

        void queueCloseAssetWindow(std::shared_ptr<AssetWindow> w) {
            closeAssetWindowsQueue_.emplace_back(std::move(w));
        }

        void findAssetToClassName(const std::string& className);

    protected:
        struct AssetPathNode {
            std::map<std::string, AssetPathNode> childNodes;
            std::map<std::string, std::string> assetFiles;
        };

        void buildAssetFileTree(AssetPathNode& rootNode, const std::optional<std::string>& filter = std::nullopt);

        void drawAssetFileTree(const AssetPathNode& node);

        void openAssetWindow(const std::string& assetFilename);

        std::shared_ptr<DataView> dataView_;
        AssetPathNode rootNode_;
        AssetPathNode rootNodeFiltered_;
        bool show_;

        std::vector<std::shared_ptr<AssetWindow>> assetWindows_;
        std::vector<std::shared_ptr<AssetWindow>> closeAssetWindowsQueue_;
    };
} // namespace Satisfactory3DMap
