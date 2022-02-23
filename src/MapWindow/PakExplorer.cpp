#include "PakExplorer.h"

#include <imgui.h>
#include <imgui_memory_editor.h>

#include "Utils/ImGuiUtil.h"
#include "Utils/StringUtils.h"

Satisfactory3DMap::PakExplorer::PakExplorer(std::shared_ptr<DataView> dataView)
    : dataView_(std::move(dataView)),
      show_(false) {
    if (dataView_->pak() == nullptr) {
        return;
    }

    // Store assets into a tree structure
    const auto& assetFileNames = dataView_->pak()->getAllAssetFilenames();
    for (const auto& asset : assetFileNames) {
        std::filesystem::path p(asset);
        std::string filename = p.filename().string();
        std::reference_wrapper<AssetPathNode> n = rootNode_;
        for (const auto& dir : p.parent_path()) {
            n = n.get().childNodes[dir.string()];
        }
        if (n.get().assetFiles.count(filename) > 0) {
            throw std::runtime_error("Asset filename is not unique!");
        }
        n.get().assetFiles[filename] = asset;
    }
}

void Satisfactory3DMap::PakExplorer::renderGui() {
    if (!show_ || dataView_->pak() == nullptr) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(300.0f, 400.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(100.0f, 100.0f), ImGuiCond_Once);
    ImGui::Begin("Pak Explorer", &show_);
    ImGui::Indent(ImGuiUtil::extraIndentWidthTreeNode);
    drawAssetFileTree(rootNode_);
    ImGui::Unindent(ImGuiUtil::extraIndentWidthTreeNode);
    ImGui::End();

    bool showFileView = !selectedAssetFile_.empty();
    if (showFileView) {
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(400.0f, 100.0f), ImGuiCond_Once);
        ImGui::Begin("Asset File View", &showFileView);
        ImGui::Text("%s", selectedAssetFile_.c_str());
        if (asset_ != nullptr) {
            if (ImGui::CollapsingHeader("Summary")) {
                const auto& sum = asset_->summary();
                ImGui::Text("Tag: %x", sum.Tag);
                ImGui::Text("LegacyFileVersion: %i", sum.LegacyFileVersion);
                ImGui::Text("LegacyUE3Version: %i", sum.LegacyUE3Version);
                ImGui::Text("FileVersionUE4: %i", sum.FileVersionUE4);
                ImGui::Text("FileVersionLicenseeUE4: %i", sum.FileVersionLicenseeUE4);
                ImGui::Text("TotalHeaderSize: %i", sum.TotalHeaderSize);
                ImGui::Text("FolderName: %s", sum.FolderName.c_str());
                ImGui::Text("PackageFlags: %i", sum.PackageFlags);
                ImGui::Text("NameCount: %i", sum.NameCount);
                ImGui::Text("NameOffset: %i", sum.NameOffset);
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
            }
            if (ImGui::CollapsingHeader("Name Map")) {
                if (ImGui::Button("Copy")) {
                    ImGui::SetClipboardText(asset_->nameMapToString().c_str());
                }
                ImGui::Text("%s", asset_->nameMapToString().c_str());
            }
            if (ImGui::CollapsingHeader("Import Map")) {
                int i = 0;
                for (const auto& importEntry : asset_->importMap()) {
                    ImGui::Text("[%i]", i);
                    ImGui::Text("ClassPackage: %s", importEntry.ClassPackage.Name.c_str());
                    ImGui::Text("ClassName: %s", importEntry.ClassName.Name.c_str());
                    ImGui::Text("OuterIndex: %i", importEntry.OuterIndex);
                    ImGui::Text("ObjectName: %s", importEntry.ObjectName.Name.c_str());
                    ImGui::Separator();
                    i++;
                }
            }
            if (ImGui::CollapsingHeader("Export Map")) {
                int i = 0;
                for (const auto& exportEntry : asset_->exportMap()) {
                    if (ImGui::Button(("View##" + std::to_string(i)).c_str())) {
                        showExport(i);
                    }
                    ImGui::Text("[%i]", i);
                    ImGui::Text("ClassIndex: %i", exportEntry.ClassIndex);
                    ImGui::Text("SuperIndex: %i", exportEntry.SuperIndex);
                    ImGui::Text("TemplateIndex: %i", exportEntry.TemplateIndex);
                    ImGui::Text("OuterIndex: %i", exportEntry.OuterIndex);
                    ImGui::Text("ObjectName: %s", exportEntry.ObjectName.Name.c_str());
                    ImGui::Text("Save: %i", exportEntry.Save);
                    ImGui::Text("SerialSize: %lli", exportEntry.SerialSize);
                    ImGui::Text("SerialOffset: %lli", exportEntry.SerialOffset);
                    ImGui::Text("bForcedExport: %i", exportEntry.bForcedExport);
                    ImGui::Text("bNotForClient: %i", exportEntry.bNotForClient);
                    ImGui::Text("bNotForServer: %i", exportEntry.bNotForServer);
                    ImGui::Text("PackageGuid: %s", exportEntry.PackageGuid.toString().c_str());
                    ImGui::Text("PackageFlags: %i", exportEntry.PackageFlags);
                    ImGui::Text("bNotAlwaysLoadedForEditorGame: %i", exportEntry.bNotAlwaysLoadedForEditorGame);
                    ImGui::Text("bIsAsset: %i", exportEntry.bIsAsset);
                    ImGui::Text("FirstExportDependency: %i", exportEntry.FirstExportDependency);
                    ImGui::Text("SerializationBeforeSerializationDependencies: %i",
                        exportEntry.SerializationBeforeSerializationDependencies);
                    ImGui::Text("CreateBeforeSerializationDependencies: %i",
                        exportEntry.CreateBeforeSerializationDependencies);
                    ImGui::Text("SerializationBeforeCreateDependencies: %i",
                        exportEntry.SerializationBeforeCreateDependencies);
                    ImGui::Text("CreateBeforeCreateDependencies: %i", exportEntry.CreateBeforeCreateDependencies);
                    ImGui::Separator();
                    i++;
                }
            }
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
        ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(800.0f, 100.0f), ImGuiCond_Once);
        ImGui::Begin("Asset Object View", &showFileView);
        if (ImGui::CollapsingHeader("Properties")) {
            propertyRenderer_.renderGui(assetExport_->properties, [&](const std::string& p) {});
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
    const std::string assetName = classNameToAssetPath(className);
    if (dataView_->pak()->containsAssetFilename(assetName)) {
        selectAsset(assetName);
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
    assetExport_.reset();
    selectedAssetFile_ = "";
    if (assetFilename.empty()) {
        return;
    }
    if (!dataView_->pak()->containsAssetFilename(assetFilename)) {
        throw std::runtime_error("Missing asset file!");
    }
    selectedAssetFile_ = assetFilename;
    if (std::filesystem::path(selectedAssetFile_).extension().string() == ".uasset") {
        asset_ = std::make_unique<AssetFile>(dataView_->pak()->readAsset(assetFilename));
    }
}

void Satisfactory3DMap::PakExplorer::showExport(int idx) {
    if (asset_ == nullptr) {
        return;
    }
    const auto exportEntry = asset_->exportMap().at(idx);
    asset_->seek(exportEntry.SerialOffset);
    assetExport_ = std::make_unique<AssetExport>();
    assetExport_->binary = asset_->read_vector<char>(exportEntry.SerialSize);
    asset_->seek(exportEntry.SerialOffset);
    *asset_ << assetExport_->properties;
    assetExport_->propertiesBinSize = asset_->tell() - exportEntry.SerialOffset;
}
