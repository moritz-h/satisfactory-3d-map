#pragma once

#include <memory>
#include <string>
#include <vector>

#include "DataView/DataView.h"
#include "GameTypes/Properties/Properties.h"
#include "UI/PropertyTableGuiRenderer.h"

namespace Satisfactory3DMap {

    class PakExplorer {
    public:
        explicit PakExplorer(std::shared_ptr<DataView> dataView);
        ~PakExplorer() = default;

        void renderGui();

        bool& show() {
            return show_;
        }

        void findAssetToClassName(const std::string& className);

    protected:
        struct AssetPathNode {
            std::map<std::string, AssetPathNode> childNodes;
            std::map<std::string, std::string> assetFiles;
        };

        struct AssetExport {
            std::vector<char> binary;
            std::size_t propertiesBinSize = 0;
            Properties properties;
            std::string propertiesError;
        };

        void drawAssetFileTree(const AssetPathNode& node);

        void selectAsset(const std::string& assetFile);

        void showExport(int idx);

        std::shared_ptr<DataView> dataView_;
        PropertyTableGuiRenderer propertyRenderer_;
        AssetPathNode rootNode_;
        bool show_;
        std::string selectedAssetFile_;
        std::unique_ptr<AssetFile> asset_;
        std::unique_ptr<AssetExport> assetExport_;
    };
} // namespace Satisfactory3DMap
