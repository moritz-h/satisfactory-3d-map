#include "PakExplorer.h"

#include <imgui.h>
#include <imgui_memory_editor.h>
#include <imgui_stdlib.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/Utils/StringUtils.h"

#include "Utils/FileDialogUtil.h"
#include "Utils/ImGuiUtil.h"

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

    ImGui::SetNextWindowSize(ImVec2(400.0f, 600.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(100.0f, 100.0f), ImGuiCond_Once);
    ImGui::Begin("Pak Explorer", &show_);
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
    ImGui::Indent(ImGuiUtil::extraIndentWidthTreeNode);
    drawAssetFileTree(searchStr.empty() ? rootNode_ : rootNodeFiltered_);
    ImGui::Unindent(ImGuiUtil::extraIndentWidthTreeNode);
    ImGui::End();

    bool showFileView = !selectedAssetFile_.empty();
    if (showFileView) {
        ImGui::SetNextWindowSize(ImVec2(500.0f, 600.0f), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(500.0f, 100.0f), ImGuiCond_Once);
        ImGui::Begin("Asset File View", &showFileView);
        ImGui::Text("%s", selectedAssetFile_.c_str());
        ImGui::SameLine();
        if (ImGui::SmallButton("Export")) {
            auto file = saveFile("Save asset", SatisfactorySave::splitPathName(selectedAssetFile_).back());
            if (file.has_value()) {
                const auto data = dataView_->pakManager()->readAssetFileContent(selectedAssetFile_);
                std::ofstream f(file.value(), std::ios::binary);
                f.write(data.data(), data.size());
            }
        }
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
                ImGui::Text("BulkDataStartOffset: %lli", sum.BulkDataStartOffset);
                ImGui::Text("WorldTileInfoDataOffset: %i", sum.WorldTileInfoDataOffset);
                ImGui::Text("ChunkIDs: TODO"); // std::vector<int32_t> ChunkIDs;
                ImGui::Text("PreloadDependencyCount: %i", sum.PreloadDependencyCount);
                ImGui::Text("PreloadDependencyOffset: %i", sum.PreloadDependencyOffset);
                ImGui::Text("NamesReferencedFromExportDataCount: %i", sum.NamesReferencedFromExportDataCount);
                ImGui::Text("PayloadTocOffset: %lli", sum.PayloadTocOffset);
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
                static int pageIdx = 0;
                const int numPages = (static_cast<int>(exportMap.size()) + pageSize - 1) / pageSize;
                if (numPages > 1) {
                    ImGui::Text("Page %i/%i", pageIdx + 1, numPages);
                    ImGui::SameLine();
                    if (ImGui::Button("|<")) {
                        pageIdx = 0;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("-10")) {
                        pageIdx -= 10;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("-1")) {
                        pageIdx--;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("+1")) {
                        pageIdx++;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("+10")) {
                        pageIdx += 10;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(">|")) {
                        pageIdx = numPages - 1;
                    }
                    ImGui::Separator();
                }
                pageIdx = std::clamp(pageIdx, 0, numPages - 1);

                const int maxElement = std::min((pageIdx + 1) * pageSize, static_cast<int>(exportMap.size()));
                for (int i = pageIdx * pageSize; i < maxElement; i++) {
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
                    ImGui::Text("SerialSize: %lli", exportEntry.SerialSize);
                    ImGui::Text("SerialOffset: %lli", exportEntry.SerialOffset);
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
        if (!showFileView) {
            selectAsset("");
        }
    }

    bool showObjectView = assetExport_ != nullptr;
    if (showObjectView) {
        ImGui::SetNextWindowSize(ImVec2(500.0f, 600.0f), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(1000.0f, 100.0f), ImGuiCond_Once);
        ImGui::Begin("Asset Object View", &showFileView);
        if (ImGui::CollapsingHeader("Properties")) {
            if (assetExport_->propertiesError.empty()) {
                propertyRenderer_.renderGui(assetExport_->properties, [&]([[maybe_unused]] const std::string& p) {});
            } else {
                ImGui::Text("Error parsing properties:");
                ImGui::Text("%s", assetExport_->propertiesError.c_str());
            }
        }
        if (ImGui::CollapsingHeader("Hex")) {
            const char* items[] = {"Full", "Properties", "After Prop."};
            static int item_current = 0;
            ImGui::Combo("Show Hex", &item_current, items, IM_ARRAYSIZE(items));
            auto& bin = assetExport_->binary;
            const auto& propBinSize = assetExport_->propertiesBinSize;
            void* data = bin.data();
            std::size_t size = bin.size();
            if (item_current == 1) {
                data = bin.data();
                size = propBinSize;
            } else if (item_current == 2) {
                data = bin.data() + propBinSize;
                size = bin.size() - propBinSize;
            }
            static MemoryEditor hexEditor;
            hexEditor.ReadOnly = true;
            hexEditor.DrawContents(data, size);
        }
        ImGui::End();
        if (!showFileView) {
            assetExport_.reset();
        }
    }
}

void Satisfactory3DMap::PakExplorer::findAssetToClassName(const std::string& className) {
    const std::string assetName = SatisfactorySave::PakManager::classNameToAssetPath(className);
    if (dataView_->pakManager()->containsAssetFilename(assetName)) {
        selectAsset(assetName);
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
        if (n.get().assetFiles.count(filename) > 0) {
            throw std::runtime_error("Asset filename is not unique!");
        }
        n.get().assetFiles[filename] = asset;
    }
}

void Satisfactory3DMap::PakExplorer::drawAssetFileTree(const AssetPathNode& node) {
    ImGui::Unindent(ImGuiUtil::extraIndentWidthTreeNode);
    for (const auto& child : node.childNodes) {
        if (ImGui::TreeNode((child.first).c_str())) {
            drawAssetFileTree(child.second);
            ImGui::TreePop();
        }
    }
    ImGui::Unindent(ImGuiUtil::extraIndentWidthLeafNode);
    for (const auto& assetPair : node.assetFiles) {
        const auto& assetFilename = assetPair.second;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (assetFilename == selectedAssetFile_) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        ImGui::TreeNodeEx(assetFilename.c_str(), flags, "%s", assetPair.first.c_str());
        if (ImGui::IsItemClicked()) {
            selectAsset(assetFilename);
        }
    }
    ImGui::Indent(ImGuiUtil::extraIndentWidthTreeNode + ImGuiUtil::extraIndentWidthLeafNode);
}

void Satisfactory3DMap::PakExplorer::selectAsset(const std::string& assetFilename) {
    asset_.reset();
    assetError_.clear();
    assetExport_.reset();
    selectedAssetFile_ = "";
    if (assetFilename.empty()) {
        return;
    }
    if (!dataView_->pakManager()->containsAssetFilename(assetFilename)) {
        throw std::runtime_error("Missing asset file!");
    }
    selectedAssetFile_ = assetFilename;
    const auto ext = std::filesystem::path(selectedAssetFile_).extension().string();
    if (ext == ".uasset" || ext == ".umap") {
        try {
            asset_ = std::make_unique<SatisfactorySave::AssetFile>(dataView_->pakManager()->readAsset(assetFilename));
        } catch (const std::exception& ex) {
            spdlog::error("Error parsing asset: {}", ex.what());
            assetError_ = ex.what();
        }
    }
}

void Satisfactory3DMap::PakExplorer::showExport(int idx) {
    if (asset_ == nullptr) {
        return;
    }
    const auto exportEntry = asset_->exportMap().at(idx);
    asset_->seek(exportEntry.SerialOffset);
    assetExport_ = std::make_unique<AssetExport>();
    assetExport_->binary = asset_->read_buffer(exportEntry.SerialSize);
    try {
        asset_->seek(exportEntry.SerialOffset);
        *asset_ << assetExport_->properties;
        assetExport_->propertiesBinSize = asset_->tell() - exportEntry.SerialOffset;
    } catch (const std::exception& ex) {
        spdlog::error("Error parsing asset properties: {}", ex.what());
        // Reset properties
        auto tmp = std::move(assetExport_);
        assetExport_ = std::make_unique<AssetExport>();
        assetExport_->binary = std::move(tmp->binary);
        assetExport_->propertiesError = ex.what();
    }
}

void Satisfactory3DMap::PakExplorer::exportExport(int idx) {
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
