#include "PakExplorer.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/Utils/StringUtils.h"

#include "MapWindow/UI/ObjectWidgets.h"
#include "Utils/FileDialogUtil.h"

Satisfactory3DMap::PakExplorer::PakExplorer(std::shared_ptr<DataView> dataView)
    : dataView_(std::move(dataView)),
      show_(false) {
    if (dataView_->pakManager() == nullptr) {
        return;
    }

    // Store assets into a tree structure
    buildAssetFileTree(rootNode_);
}

void Satisfactory3DMap::PakExplorer::renderGui() {
    if (!show_ || dataView_->pakManager() == nullptr) {
        return;
    }

    for (auto& close : closeAssetWindowsQueue_) {
        std::erase(assetWindows_, close);
    }
    closeAssetWindowsQueue_.clear();

    ImGui::SetNextWindowSize(ImVec2(400.0f, 600.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(100.0f, 100.0f), ImGuiCond_Once);
    ImGui::Begin("Pak Explorer", &show_);
    if (ImGui::CollapsingHeader("GlobalNameMap")) {
        const auto globalNameMapStr = dataView_->pakManager()->getGlobalNameMap().toString();
        if (ImGui::Button("Copy")) {
            ImGui::SetClipboardText(globalNameMapStr.c_str());
        }
        ImGui::TextUnformatted(globalNameMapStr.c_str());
    }
    if (ImGui::CollapsingHeader("GlobalScriptObjectEntries")) {
        const auto& globalNameMap = dataView_->pakManager()->getGlobalNameMap();
        const auto& scriptObjectEntries = dataView_->pakManager()->getScriptObjectEntries();
        for (std::size_t i = 0; i < scriptObjectEntries.size(); i++) {
            const auto& entry = scriptObjectEntries[i];
            ImGui::Text("[%zu]", i);
            ImGui::Text("Mapped: %s", globalNameMap.GetName(entry.Mapped).toString().c_str());
            UI::ShowPackageObjectIndex("GlobalIndex", entry.GlobalIndex, dataView_->pakManager());
            UI::ShowPackageObjectIndex("OuterIndex", entry.OuterIndex, dataView_->pakManager());
            UI::ShowPackageObjectIndex("CDOClassIndex", entry.CDOClassIndex, dataView_->pakManager());
            ImGui::Separator();
        }
    }
    if (ImGui::CollapsingHeader("Content", ImGuiTreeNodeFlags_DefaultOpen)) {
        static std::string searchStr;
        ImGui::Text("Search/Filter:");
        ImGui::SameLine();
        if (ImGui::InputText("##Search", &searchStr)) {
            if (!searchStr.empty()) {
                rootNodeFiltered_ = AssetPathNode();
                buildAssetFileTree(rootNodeFiltered_, searchStr);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            searchStr.clear();
        }
        ImGui::Separator();
        drawAssetFileTree(searchStr.empty() ? rootNode_ : rootNodeFiltered_);
    }
    ImGui::End();

    for (auto& assetWindow : assetWindows_) {
        assetWindow->renderGui();
    }
}

void Satisfactory3DMap::PakExplorer::findAssetToClassName(const std::string& className) {
    const std::string assetName = SatisfactorySave::PakManager::classNameToAssetPath(className);
    if (dataView_->pakManager()->containsAssetFilename(assetName)) {
        openAssetWindow(assetName);
    }
}

void Satisfactory3DMap::PakExplorer::buildAssetFileTree(AssetPathNode& rootNode,
    const std::optional<std::string>& filter) {
    const auto& assetFileNames = dataView_->pakManager()->getAllAssetFilenames();
    for (const auto& asset : assetFileNames) {
        if (filter.has_value()) {
            if (!SatisfactorySave::containsCaseInsensitive(asset, filter.value())) {
                continue;
            }
        }
        std::filesystem::path p(asset);
        std::string filename = p.filename().string();
        std::reference_wrapper<AssetPathNode> n = rootNode;
        for (const auto& dir : p.parent_path()) {
            n = n.get().childNodes[dir.string()];
        }
        if (n.get().assetFiles.contains(filename)) {
            throw std::runtime_error("Asset filename is not unique!");
        }
        n.get().assetFiles[filename] = asset;
    }
}

void Satisfactory3DMap::PakExplorer::drawAssetFileTree(const AssetPathNode& node) {
    for (const auto& child : node.childNodes) {
        if (ImGui::TreeNode((child.first).c_str())) {
            drawAssetFileTree(child.second);
            ImGui::TreePop();
        }
    }
    for (const auto& assetPair : node.assetFiles) {
        const auto& assetFilename = assetPair.second;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx(assetFilename.c_str(), flags, "%s", assetPair.first.c_str());
        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            openAssetWindow(assetFilename);
        }
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::Button("Open")) {
                openAssetWindow(assetFilename);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Export")) {
                auto file = saveFile("Save asset", SatisfactorySave::splitPathName(assetFilename).back());
                if (file.has_value()) {
                    const auto data = dataView_->pakManager()->readAssetFileContent(assetFilename);
                    std::ofstream f(file.value(), std::ios::binary);
                    f.write(data.data(), data.size());
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void Satisfactory3DMap::PakExplorer::openAssetWindow(const std::string& assetFilename) {
    if (assetFilename.empty()) {
        return;
    }
    if (!dataView_->pakManager()->containsAssetFilename(assetFilename)) {
        throw std::runtime_error("Missing asset file!");
    }
    const auto ext = std::filesystem::path(assetFilename).extension().string();
    if (ext == ".uasset" || ext == ".umap") {
        std::shared_ptr<SatisfactorySave::AssetFile> asset;
        std::string assetError;
        try {
            asset = std::make_shared<SatisfactorySave::AssetFile>(dataView_->pakManager()->readAsset(assetFilename));
        } catch (const std::exception& ex) {
            spdlog::error("Error parsing asset: {}", ex.what());
            assetError = ex.what();
        }
        assetWindows_.emplace_back(
            std::make_shared<AssetWindow>(shared_from_this(), std::move(asset), assetFilename, std::move(assetError)));
    } else {
        assetWindows_.emplace_back(std::make_shared<AssetWindow>(shared_from_this(), nullptr, assetFilename,
            "Can only show *.uasset and *.umap!"));
    }
}
