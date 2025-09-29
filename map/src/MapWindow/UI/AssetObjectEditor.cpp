#include "AssetObjectEditor.h"

#include <imgui.h>
#include <implot3d.h>
#include <spdlog/spdlog.h>

#include "ObjectWidgets.h"

void Satisfactory3DMap::UI::AssetObjectEditor::renderGui() {
    if (assetExport_->Object != nullptr) {
        AssetUObjectEditor e(*this);
        e.dispatch(*assetExport_->Object);
    }
    if (!assetExport_->BinaryClassData.empty()) {
        EditorSectionWrap(0, "Binary Class Data", [&]() {
            EditorShowBinData("Unknown Class Data", assetExport_->BinaryClassData, ctx_);
        });
    }
}

void Satisfactory3DMap::UI::AssetObjectEditor::AssetUObjectEditor::visit(s::UObject& o) {
    EditorSectionWrap(0, "UObject", [&]() {
        EditorPropertyList("Properties", o.Properties, parent_.ctx_);
        EditorOptional("Guid", o.Guid, [&](auto& item) {
            EditorShowText("Value", "TODO FGuid!"); // TODO item
        });
    });
}

void Satisfactory3DMap::UI::AssetObjectEditor::AssetUObjectEditor::visit(s::UStaticMesh& o) {
    visit(static_cast<s::UObject&>(o));
    if (ImGui::CollapsingHeader("UStaticMesh", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (!parent_.staticMesh_error_.has_value()) {
            if (parent_.staticMesh_ == nullptr) {
                try {
                    parent_.staticMesh_ = std::make_shared<PlotStaticMesh>(o);
                } catch (const std::exception& ex) {
                    parent_.staticMesh_error_ = ex.what();
                    spdlog::error("Error reading UStaticMesh: {}", ex.what());
                }
            }
            if (parent_.staticMesh_ != nullptr) {
                static constexpr ImVec4 line_color = ImVec4(0.5f, 0.5f, 0.2f, 0.6f);
                static constexpr ImVec4 fill_color = ImVec4(0.8f, 0.8f, 0.2f, 0.6f);
                static constexpr ImVec4 marker_color = ImVec4(0.5f, 0.5f, 0.2f, 0.6f);
                static constexpr ImPlot3DAxisFlags axisFlags = ImPlot3DAxisFlags_Lock;
                static constexpr ImPlot3DMeshFlags meshFlags = ImPlot3DMeshFlags_NoMarkers;

                if (ImPlot3D::BeginPlot("Mesh")) {
                    const auto& cube = parent_.staticMesh_->cube();
                    const auto& vert = parent_.staticMesh_->vertices();
                    const auto& idx = parent_.staticMesh_->indices();

                    ImPlot3D::SetupAxes("X", "Y", "Z", axisFlags, axisFlags, axisFlags);
                    ImPlot3D::SetupAxesLimits(cube.Min.x, cube.Max.x, cube.Min.y, cube.Max.y, cube.Min.z, cube.Max.z);
                    ImPlot3D::SetNextFillStyle(fill_color);
                    ImPlot3D::SetNextLineStyle(line_color);
                    ImPlot3D::SetNextMarkerStyle(ImPlot3DMarker_Square, 3, marker_color, IMPLOT3D_AUTO, marker_color);
                    ImPlot3D::PlotMesh("LOD[0]", vert.data(), idx.data(), static_cast<int>(vert.size()),
                        static_cast<int>(idx.size()), meshFlags);
                    ImPlot3D::EndPlot();
                }
            }
        } else {
            ImGui::Text("UStaticMesh Error: %s", parent_.staticMesh_error_.value().c_str());
        }
    }
}

void Satisfactory3DMap::UI::AssetObjectEditor::AssetUObjectEditor::visit(s::UTexture2D& o) {
    visit(static_cast<s::UObject&>(o));
    if (ImGui::CollapsingHeader("UTexture2D", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (!parent_.texture2d_error_.has_value()) {
            if (parent_.texture2d_ == nullptr) {
                try {
                    parent_.texture2d_ = std::make_unique<OGLTexture2D>(o);
                } catch (const std::exception& ex) {
                    parent_.texture2d_error_ = ex.what();
                    spdlog::error("Error reading UTexture2D: {}", ex.what());
                }
            }
            if (parent_.texture2d_ != nullptr) {
                ImVec2 size = ImGui::GetContentRegionAvail();
                size.y = size.x / static_cast<float>(parent_.texture2d_->sizeX()) *
                         static_cast<float>(parent_.texture2d_->sizeY());
                ImGui::Image(static_cast<ImTextureID>(parent_.texture2d_->name()), size);
            }
        } else {
            ImGui::Text("UTexture2D Error: %s", parent_.texture2d_error_.value().c_str());
        }
    }
}
