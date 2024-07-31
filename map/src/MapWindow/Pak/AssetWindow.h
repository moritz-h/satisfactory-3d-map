#pragma once

#include <memory>
#include <string>

#include "SatisfactorySave/Pak/AssetFile.h"

#include "AssetObjectWindow.h"

namespace Satisfactory3DMap {

    class PakExplorer;

    class AssetWindow : public std::enable_shared_from_this<AssetWindow> {
    public:
        explicit AssetWindow(std::shared_ptr<PakExplorer> pakExplorer, std::shared_ptr<SatisfactorySave::AssetFile> asset, std::string assetFilename,
            std::string assetError);

        void renderGui();

        void queueCloseExportWindow(std::shared_ptr<AssetObjectWindow> w) {
            closeExportWindowsQueue_.emplace_back(std::move(w));
        }

    protected:
        void showExport(int idx);

        void exportExport(int idx);

        std::shared_ptr<PakExplorer> pakExplorer_;
        std::shared_ptr<SatisfactorySave::AssetFile> asset_;
        std::string assetFilename_;
        std::string assetError_;
        std::string windowTitle_;

        std::vector<std::shared_ptr<AssetObjectWindow>> exportWindows_;
        std::vector<std::shared_ptr<AssetObjectWindow>> closeExportWindowsQueue_;
    };
} // namespace Satisfactory3DMap
