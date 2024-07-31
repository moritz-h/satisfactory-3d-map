#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"

#include "../UI/PropertyTableGuiRenderer.h"

namespace Satisfactory3DMap {

    class AssetWindow;

    class AssetObjectWindow : public std::enable_shared_from_this<AssetObjectWindow> {
    public:
        struct AssetExport {
            std::vector<char> binary;
            std::size_t propertiesBinSize = 0;
            SatisfactorySave::PropertyList properties;
            std::string propertiesError;
        };

        AssetObjectWindow(std::shared_ptr<AssetWindow> assetWindow, std::unique_ptr<AssetExport> assetExport);

        void renderGui();

    protected:
        std::shared_ptr<AssetWindow> assetWindow_;
        std::unique_ptr<AssetExport> assetExport_;
        std::string windowTitle_;
        PropertyTableGuiRenderer propertyRenderer_;
    };
} // namespace Satisfactory3DMap
