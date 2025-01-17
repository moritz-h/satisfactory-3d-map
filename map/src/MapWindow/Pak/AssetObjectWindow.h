#pragma once

#include <memory>
#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_memory_editor.h>

#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"

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

        AssetObjectWindow(std::shared_ptr<AssetWindow> assetWindow, std::unique_ptr<AssetExport> assetExport,
            const std::string& title);

        void renderGui();

    protected:
        std::shared_ptr<AssetWindow> assetWindow_;
        std::unique_ptr<AssetExport> assetExport_;
        std::string windowTitle_;
        MemoryEditor hexEditor_;
        int hexEditorMode_ = 2;
    };
} // namespace Satisfactory3DMap
