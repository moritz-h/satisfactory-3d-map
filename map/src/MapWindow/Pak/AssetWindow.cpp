#include "AssetWindow.h"

#include <cinttypes>

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/Utils/StringUtils.h"

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
    ImGui::PushID(this);
    ImGui::SetNextWindowSize(ImVec2(500.0f, 600.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(500.0f, 100.0f), ImGuiCond_Once);
    bool open = true;
    ImGui::Begin(windowTitle_.c_str(), &open);
    ImGui::Text("%s", assetFilename_.c_str());
    if (asset_ != nullptr) {
        if (ImGui::CollapsingHeader("Summary")) {
            const auto& sum = asset_->summary();
            ImGui::Text("Tag: %x", sum.Tag);
            ImGui::Text("LegacyFileVersion: %i", sum.LegacyFileVersion);
            ImGui::Text("LegacyUE3Version: %i", sum.LegacyUE3Version);
            ImGui::Text("FileVersionUE4: %i", sum.FileVersionUE4);
            ImGui::Text("FileVersionUE5: %i", sum.FileVersionUE5);
            ImGui::Text("FileVersionLicenseeUE: %i", sum.FileVersionLicenseeUE);
            ImGui::Text("TotalHeaderSize: %i", sum.TotalHeaderSize);
            ImGui::Text("PackageName: %s", sum.PackageName.c_str());
            ImGui::Text("PackageFlags: %i", sum.PackageFlags);
            ImGui::Text("NameCount: %i", sum.NameCount);
            ImGui::Text("NameOffset: %i", sum.NameOffset);
            ImGui::Text("SoftObjectPathsCount: %i", sum.SoftObjectPathsCount);
            ImGui::Text("SoftObjectPathsOffset: %i", sum.SoftObjectPathsOffset);
            ImGui::Text("GatherableTextDataCount: %i", sum.GatherableTextDataCount);
            ImGui::Text("GatherableTextDataOffset: %i", sum.GatherableTextDataOffset);
            ImGui::Text("ExportCount: %i", sum.ExportCount);
            ImGui::Text("ExportOffset: %i", sum.ExportOffset);
            ImGui::Text("ImportCount: %i", sum.ImportCount);
            ImGui::Text("ImportOffset: %i", sum.ImportOffset);
            ImGui::Text("DependsOffset: %i", sum.DependsOffset);
            ImGui::Text("SoftPackageReferencesCount: %i", sum.SoftPackageReferencesCount);
            ImGui::Text("SoftPackageReferencesOffset: %i", sum.SoftPackageReferencesOffset);
            ImGui::Text("SearchableNamesOffset: %i", sum.SearchableNamesOffset);
            ImGui::Text("ThumbnailTableOffset: %i", sum.ThumbnailTableOffset);
            ImGui::Text("Guid: %s", sum.Guid.toString().c_str());
            ImGui::Text("Generations: TODO");                 // std::vector<FGenerationInfo> Generations;
            ImGui::Text("SavedByEngineVersion: TODO");        // EngineVersion SavedByEngineVersion;
            ImGui::Text("CompatibleWithEngineVersion: TODO"); // EngineVersion CompatibleWithEngineVersion;
            ImGui::Text("CompressionFlags: %i", sum.CompressionFlags);
            ImGui::Text("PackageSource: %i", sum.PackageSource);
            ImGui::Text("AssetRegistryDataOffset: %i", sum.AssetRegistryDataOffset);
            ImGui::Text("BulkDataStartOffset: %" PRIi64, sum.BulkDataStartOffset);
            ImGui::Text("WorldTileInfoDataOffset: %i", sum.WorldTileInfoDataOffset);
            ImGui::Text("ChunkIDs: TODO"); // std::vector<int32_t> ChunkIDs;
            ImGui::Text("PreloadDependencyCount: %i", sum.PreloadDependencyCount);
            ImGui::Text("PreloadDependencyOffset: %i", sum.PreloadDependencyOffset);
            ImGui::Text("NamesReferencedFromExportDataCount: %i", sum.NamesReferencedFromExportDataCount);
            ImGui::Text("PayloadTocOffset: %" PRIi64, sum.PayloadTocOffset);
        }
        if (ImGui::CollapsingHeader("Name Map")) {
            if (ImGui::Button("Copy")) {
                ImGui::SetClipboardText(asset_->nameMapToString().c_str());
            }
            ImGui::TextUnformatted(asset_->nameMapToString().c_str());
        }
        if (ImGui::CollapsingHeader("Import Map")) {
            int i = 0;
            for (const auto& importEntry : asset_->importMap()) {
                ImGui::Text("[%i]", i);
                ImGui::Text("ClassPackage: %s", importEntry.ClassPackage.toString().c_str());
                ImGui::Text("ClassName: %s", importEntry.ClassName.toString().c_str());
                ImGui::Text("OuterIndex: %i", importEntry.OuterIndex);
                ImGui::Text("ObjectName: %s", importEntry.ObjectName.toString().c_str());
                ImGui::Text("bImportOptional: %i", importEntry.bImportOptional);
                ImGui::Separator();
                i++;
            }
        }
        if (ImGui::CollapsingHeader("Export Map")) {
            const auto& exportMap = asset_->exportMap();

            constexpr int pageSize = 1000;
            const int numPages = (static_cast<int>(exportMap.size()) + pageSize - 1) / pageSize;
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
                ImGui::Text("ClassIndex: %i", exportEntry.ClassIndex);
                ImGui::Text("SuperIndex: %i", exportEntry.SuperIndex);
                ImGui::Text("TemplateIndex: %i", exportEntry.TemplateIndex);
                ImGui::Text("OuterIndex: %i", exportEntry.OuterIndex);
                ImGui::Text("ObjectName: %s", exportEntry.ObjectName.toString().c_str());
                ImGui::Text("Save: %i", exportEntry.Save);
                ImGui::Text("SerialSize: %" PRIi64, exportEntry.SerialSize);
                ImGui::Text("SerialOffset: %" PRIi64, exportEntry.SerialOffset);
                ImGui::Text("bForcedExport: %i", exportEntry.bForcedExport);
                ImGui::Text("bNotForClient: %i", exportEntry.bNotForClient);
                ImGui::Text("bNotForServer: %i", exportEntry.bNotForServer);
                ImGui::Text("bIsInheritedInstance: %i", exportEntry.bIsInheritedInstance);
                ImGui::Text("PackageFlags: %i", exportEntry.PackageFlags);
                ImGui::Text("bNotAlwaysLoadedForEditorGame: %i", exportEntry.bNotAlwaysLoadedForEditorGame);
                ImGui::Text("bIsAsset: %i", exportEntry.bIsAsset);
                ImGui::Text("bGeneratePublicHash: %i", exportEntry.bGeneratePublicHash);
                ImGui::Text("FirstExportDependency: %i", exportEntry.FirstExportDependency);
                ImGui::Text("SerializationBeforeSerializationDependencies: %i",
                    exportEntry.SerializationBeforeSerializationDependencies);
                ImGui::Text("CreateBeforeSerializationDependencies: %i",
                    exportEntry.CreateBeforeSerializationDependencies);
                ImGui::Text("SerializationBeforeCreateDependencies: %i",
                    exportEntry.SerializationBeforeCreateDependencies);
                ImGui::Text("CreateBeforeCreateDependencies: %i", exportEntry.CreateBeforeCreateDependencies);
                ImGui::Separator();
            }
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
    for (auto& close : closeExportWindowsQueue_) {
        std::erase(exportWindows_, close);
    }
    closeExportWindowsQueue_.clear();
}

void Satisfactory3DMap::AssetWindow::showExport(int idx) {
    if (asset_ == nullptr) {
        return;
    }
    const auto exportEntry = asset_->exportMap().at(idx);
    asset_->seek(exportEntry.SerialOffset);
    auto assetExport = std::make_unique<AssetObjectWindow::AssetExport>();
    assetExport->binary = asset_->read_buffer(exportEntry.SerialSize);
    try {
        asset_->seek(exportEntry.SerialOffset);
        *asset_ << assetExport->properties;
        assetExport->propertiesBinSize = asset_->tell() - exportEntry.SerialOffset;
    } catch (const std::exception& ex) {
        spdlog::error("Error parsing asset properties: {}", ex.what());
        // Reset properties
        auto tmp = std::move(assetExport);
        assetExport = std::make_unique<AssetObjectWindow::AssetExport>();
        assetExport->binary = std::move(tmp->binary);
        assetExport->propertiesError = ex.what();
    }
    exportWindows_.emplace_back(std::make_shared<AssetObjectWindow>(shared_from_this(), std::move(assetExport),
        exportEntry.ObjectName.toString() + " [" + SatisfactorySave::splitPathName(assetFilename_).back() + "]"));
}

void Satisfactory3DMap::AssetWindow::exportExport(int idx) {
    if (asset_ == nullptr) {
        return;
    }
    const auto exportEntry = asset_->exportMap().at(idx);
    auto file = saveFile("Save asset export", exportEntry.ObjectName.toString() + ".bin");
    if (file.has_value()) {
        asset_->seek(exportEntry.SerialOffset);
        const auto data = asset_->read_buffer(exportEntry.SerialSize);
        std::ofstream f(file.value(), std::ios::binary);
        f.write(data.data(), data.size());
    }
}
