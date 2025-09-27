#include "AssetWindow.h"

#include <cinttypes>

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/Utils/StringUtils.h"

#include "../UI/ObjectWidgets.h"
#include "PakExplorer.h"
#include "Utils/FileDialogUtil.h"

Satisfactory3DMap::AssetWindow::AssetWindow(std::shared_ptr<PakExplorer> pakExplorer,
    std::shared_ptr<SatisfactorySave::AssetFile> asset, std::string assetFilename, std::string assetError)
    : pakExplorer_(std::move(pakExplorer)),
      asset_(std::move(asset)),
      assetFilename_(std::move(assetFilename)),
      assetError_(std::move(assetError)) {
    windowTitle_ = SatisfactorySave::splitPathName(assetFilename_).back() + "###" +
                   std::to_string(reinterpret_cast<uintptr_t>(this));
}

void Satisfactory3DMap::AssetWindow::renderGui() {
    for (auto& close : closeExportWindowsQueue_) {
        std::erase(exportWindows_, close);
    }
    closeExportWindowsQueue_.clear();

    ImGui::PushID(this);
    ImGui::SetNextWindowSize(ImVec2(500.0f, 600.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(500.0f, 100.0f), ImGuiCond_Once);
    bool open = true;
    ImGui::Begin(windowTitle_.c_str(), &open);
    ImGui::Text("%s", assetFilename_.c_str());
    if (asset_ != nullptr) {
        if (ImGui::CollapsingHeader("PackageSummary")) {
            const auto& sum = asset_->summary();
            ImGui::Text("bHasVersioningInfo: %u", sum.bHasVersioningInfo);
            ImGui::Text("HeaderSize: %u", sum.HeaderSize);
            ImGui::Text("Name: %s", asset_->getNameString(sum.Name).c_str());
            ImGui::Text("PackageFlags: %u", sum.PackageFlags);
            ImGui::Text("CookedHeaderSize: %u", sum.CookedHeaderSize);
            ImGui::Text("ImportedPublicExportHashesOffset: %i", sum.ImportedPublicExportHashesOffset);
            ImGui::Text("ImportMapOffset: %i", sum.ImportMapOffset);
            ImGui::Text("ExportMapOffset: %i", sum.ExportMapOffset);
            ImGui::Text("ExportBundleEntriesOffset: %i", sum.ExportBundleEntriesOffset);
            ImGui::Text("DependencyBundleHeadersOffset: %i", sum.DependencyBundleHeadersOffset);
            ImGui::Text("DependencyBundleEntriesOffset: %i", sum.DependencyBundleEntriesOffset);
            ImGui::Text("ImportedPackageNamesOffset: %i", sum.ImportedPackageNamesOffset);
        }
        if (ImGui::CollapsingHeader("NameMap")) {
            if (ImGui::Button("Copy")) {
                ImGui::SetClipboardText(asset_->nameMap().toString().c_str());
            }
            ImGui::TextUnformatted(asset_->nameMap().toString().c_str());
        }
        if (ImGui::CollapsingHeader("ImportedPackageNames")) {
            for (std::size_t i = 0; i < asset_->importedPackageNames().size(); i++) {
                const std::string label = "[" + std::to_string(i) + "]";
                ImGui::Text("[%zu]: %s", i, asset_->importedPackageNames()[i].toString().c_str());
            }
        }
        if (ImGui::CollapsingHeader("ImportedPublicExportHashes")) {
            for (std::size_t i = 0; i < asset_->importedPublicExportHashes().size(); i++) {
                const std::string label = "[" + std::to_string(i) + "]";
                ImGui::Text("[%zu]: %" PRIu64, i, asset_->importedPublicExportHashes()[i]);
            }
        }
        if (ImGui::CollapsingHeader("ImportMap")) {
            for (std::size_t i = 0; i < asset_->importMap().size(); i++) {
                const std::string label = "[" + std::to_string(i) + "]";
                UI::ShowPackageObjectIndex(label.c_str(), asset_->importMap()[i], pakExplorer_->pakManager());
            }
        }
        if (ImGui::CollapsingHeader("ExportMap")) {
            const auto& exportMap = asset_->exportMap();

            constexpr int pageSize = 1000;
            const int numPages = std::max((static_cast<int>(exportMap.size()) + pageSize - 1) / pageSize, 1);
            if (numPages > 1) {
                ImGui::Text("Page %i/%i", exportPageIdx_ + 1, numPages);
                ImGui::SameLine();
                if (ImGui::Button("|<")) {
                    exportPageIdx_ = 0;
                }
                ImGui::SameLine();
                if (ImGui::Button("-10")) {
                    exportPageIdx_ -= 10;
                }
                ImGui::SameLine();
                if (ImGui::Button("-1")) {
                    exportPageIdx_--;
                }
                ImGui::SameLine();
                if (ImGui::Button("+1")) {
                    exportPageIdx_++;
                }
                ImGui::SameLine();
                if (ImGui::Button("+10")) {
                    exportPageIdx_ += 10;
                }
                ImGui::SameLine();
                if (ImGui::Button(">|")) {
                    exportPageIdx_ = numPages - 1;
                }
                ImGui::Separator();
            }
            exportPageIdx_ = std::clamp(exportPageIdx_, 0, numPages - 1);

            const int maxElement = std::min((exportPageIdx_ + 1) * pageSize, static_cast<int>(exportMap.size()));
            for (int i = exportPageIdx_ * pageSize; i < maxElement; i++) {
                const auto& exportEntry = exportMap[i];
                if (ImGui::Button(("View##" + std::to_string(i)).c_str())) {
                    showExport(i);
                }
                ImGui::SameLine();
                if (ImGui::Button(("Export##" + std::to_string(i)).c_str())) {
                    exportExport(i);
                }
                ImGui::Text("[%i]", i);
                ImGui::Text("CookedSerialOffset: %" PRIu64, exportEntry.CookedSerialOffset);
                ImGui::Text("CookedSerialSize: %" PRIu64, exportEntry.CookedSerialSize);
                ImGui::Text("ObjectName: %s", asset_->getNameString(exportEntry.ObjectName).c_str());
                UI::ShowPackageObjectIndex("OuterIndex", exportEntry.OuterIndex, pakExplorer_->pakManager());
                UI::ShowPackageObjectIndex("ClassIndex", exportEntry.ClassIndex, pakExplorer_->pakManager());
                UI::ShowPackageObjectIndex("SuperIndex", exportEntry.SuperIndex, pakExplorer_->pakManager());
                UI::ShowPackageObjectIndex("TemplateIndex", exportEntry.TemplateIndex, pakExplorer_->pakManager());
                ImGui::Text("PublicExportHash: %" PRIu64, exportEntry.PublicExportHash);
                ImGui::Text("ObjectFlags: %" PRIu32, exportEntry.ObjectFlags);
                ImGui::Text("FilterFlags: %" PRIu8, exportEntry.FilterFlags);
                ImGui::Separator();
            }
        }
        if (ImGui::CollapsingHeader("ExportMap Tree")) {
            // Build and cache tree structure
            if (exportChildren_.empty()) {
                const auto& exportMap = asset_->exportMap();
                exportChildren_.resize(exportMap.size());
                for (std::size_t i = 0; i < exportMap.size(); i++) {
                    const auto& outer = exportMap[i].OuterIndex;
                    if (outer.IsExport()) {
                        exportChildren_[outer.ToExport()].push_back(i);
                    } else {
                        exportRoots_.push_back(i);
                    }
                }
            }
            drawExportTree(exportRoots_);
        }
    } else if (!assetError_.empty()) {
        ImGui::Text("Error parsing asset: %s", assetError_.c_str());
    } else {
        ImGui::Text("Select *.uasset!");
    }
    ImGui::End();
    ImGui::PopID();
    if (!open) {
        pakExplorer_->queueCloseAssetWindow(shared_from_this());
    }

    for (auto& exportWindow : exportWindows_) {
        exportWindow->renderGui();
    }
}

void Satisfactory3DMap::AssetWindow::showExport(int idx) {
    if (asset_ == nullptr) {
        return;
    }
    const auto exportEntry = asset_->exportMap().at(idx);
    auto assetExport = asset_->getExportObjectByIdx(idx);

    exportWindows_.emplace_back(std::make_shared<AssetObjectWindow>(shared_from_this(), std::move(assetExport),
        asset_->getNameString(exportEntry.ObjectName) + " [" + SatisfactorySave::splitPathName(assetFilename_).back() +
            "]"));
}

void Satisfactory3DMap::AssetWindow::exportExport(int idx) {
    if (asset_ == nullptr) {
        return;
    }
    const auto exportEntry = asset_->exportMap().at(idx);
    auto file = saveFile("Save asset export", asset_->getNameString(exportEntry.ObjectName) + ".bin");
    if (file.has_value()) {
        asset_->seekCookedSerialOffset(exportEntry.CookedSerialOffset);
        const auto data = asset_->read_buffer(exportEntry.CookedSerialSize);
        std::ofstream f(file.value(), std::ios::binary);
        f.write(data.data(), data.size());
    }
}

void Satisfactory3DMap::AssetWindow::drawExportTree(const std::vector<std::size_t>& nodes) {
    for (const auto& n : nodes) {
        ImGui::PushID(static_cast<int>(n));
        const auto& exportEntry = asset_->exportMap()[n];
        std::string name = asset_->getNameString(exportEntry.ObjectName) + " [" + std::to_string(n) + "]";
        if (exportEntry.ClassIndex.IsScriptImport()) {
            name += " " + pakExplorer_->pakManager()->tryGetScriptObjectFullName(exportEntry.ClassIndex);
        }
        if (exportChildren_[n].empty()) {
            if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf)) {
                ImGui::TreePop();
            }
        } else {
            if (ImGui::TreeNodeEx(name.c_str(), 0)) {
                drawExportTree(exportChildren_[n]);
                ImGui::TreePop();
            }
        }
        ImGui::PopID();
    }
}
