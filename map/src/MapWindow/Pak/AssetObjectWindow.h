#pragma once

#include <memory>
#include <string>

#include <imgui.h>
#include <imgui_memory_editor.h>

#include "SatisfactorySave/Pak/AssetExport.h"

#include "../UI/AssetObjectEditor.h"
#include "../UI/CommonUI.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {

    class AssetWindow;

    class AssetObjectWindow : public std::enable_shared_from_this<AssetObjectWindow> {
    public:
        AssetObjectWindow(std::shared_ptr<AssetWindow> assetWindow, std::shared_ptr<s::AssetExport> assetExport,
            const std::string& title);

        void renderGui();

    protected:
        std::shared_ptr<AssetWindow> assetWindow_;
        std::shared_ptr<s::AssetExport> assetExport_;
        std::string windowTitle_;
        UI::EventContext ctx_;
        UI::AssetObjectEditor objEditor_;
        MemoryEditor hexEditor_;
        bool showHex_;
    };
} // namespace Satisfactory3DMap
