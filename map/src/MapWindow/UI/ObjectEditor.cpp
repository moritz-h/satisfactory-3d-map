#include "ObjectEditor.h"

#include <imgui.h>

#include "ObjectWidgets.h"

void Satisfactory3DMap::UI::ObjectEditor::renderGui(ObjectProxyPtr proxy) {
    // Use a shared_ptr copy as function parameter to keep object the same during drawing.
    // I.e., selection of a different object may happen during draw.

    ImGui::PushID(proxy->id());
    ImGui::Text("ID: %i, Type: %s", proxy->id(),
        proxy->isLightweight() ? "LightweightBuildable" : (proxy->isActor() ? "Actor" : "Object"));

    if (!proxy->isLightweight()) {
        const auto& saveObject = proxy->getSaveObject();

        if (ImGui::CollapsingHeader("ObjectBaseSaveHeader", ImGuiTreeNodeFlags_DefaultOpen)) {
            PushEditorTableStyle();
            if (BeginEditorTable()) {
                EditorShowSelectable("Class", saveObject->baseHeader().ClassName, ctx_);
                ImGui::BeginDisabled();
                EditorObjectReference("Reference", saveObject->baseHeader().Reference, ctx_);
                ImGui::EndDisabled();
                EndEditorTable();
            }
            PopEditorTableStyle();
        }
        if (proxy->isActor()) {
            if (ImGui::CollapsingHeader("ActorSaveHeader", ImGuiTreeNodeFlags_DefaultOpen)) {
                PushEditorTableStyle();
                if (BeginEditorTable()) {
                    if (EditorTransform(saveObject->actorHeader().Transform)) {
                        ctx_.updateTransform(proxy);
                    }
                    ImGui::BeginDisabled();
                    EditorBool("NeedTransform", saveObject->actorHeader().NeedTransform);
                    EditorBool("WasPlacedInLevel", saveObject->actorHeader().WasPlacedInLevel);
                    ImGui::EndDisabled();
                    EndEditorTable();
                }
                PopEditorTableStyle();
            }
        } else {
            if (ImGui::CollapsingHeader("ObjectSaveHeader", ImGuiTreeNodeFlags_DefaultOpen)) {
                PushEditorTableStyle();
                if (BeginEditorTable()) {
                    EditorShowSelectable("OuterPathName", saveObject->objectHeader().OuterPathName, ctx_);
                    EndEditorTable();
                }
                PopEditorTableStyle();
            }
        }
        if (ImGui::CollapsingHeader("SaveObject", ImGuiTreeNodeFlags_DefaultOpen)) {
            PushEditorTableStyle();
            if (BeginEditorTable()) {
                ImGui::BeginDisabled();
                EditorScalar("SaveVersion", ImGuiDataType_S32, &saveObject->SaveVersion);
                EditorBool("ShouldMigrate", saveObject->ShouldMigrateObjectRefsToPersistent);
                ImGui::EndDisabled();
                EndEditorTable();
            }
            PopEditorTableStyle();
        }
        // TODO
    } else {
        if (ImGui::CollapsingHeader("LightweightBuildable", ImGuiTreeNodeFlags_DefaultOpen)) {
            UI::PushEditorTableStyle();
            if (UI::BeginEditorTable()) {
                auto& instance = proxy->getLightweightData();

                UI::EditorShowSelectable("Class", proxy->className(), ctx_);
                UI::EditorShowText("Path", proxy->pathName().c_str());
                if (UI::EditorTransform(instance.Transform)) {
                    ctx_.updateTransform(proxy);
                }
                UI::EditorObjectReference("SwatchDesc", instance.CustomizationData.SwatchDesc, ctx_);
                UI::EditorObjectReference("MaterialDesc", instance.CustomizationData.MaterialDesc, ctx_);
                UI::EditorObjectReference("PatternDesc", instance.CustomizationData.PatternDesc, ctx_);
                UI::EditorObjectReference("SkinDesc", instance.CustomizationData.SkinDesc, ctx_);
                if (UI::EditorTreeNode("OverrideColorData", ImGuiTreeNodeFlags_DefaultOpen)) {
                    UI::EditorLinearColor("PrimaryColor", instance.CustomizationData.OverrideColorData.PrimaryColor);
                    UI::EditorLinearColor("SecondaryColor",
                        instance.CustomizationData.OverrideColorData.SecondaryColor);
                    UI::EditorObjectReference("PaintFinish", instance.CustomizationData.OverrideColorData.PaintFinish,
                        ctx_);
                    ImGui::TreePop();
                }
                UI::EditorScalar("PatternRotation", ImGuiDataType_U8, &instance.CustomizationData.PatternRotation);
                UI::EditorObjectReference("BuiltWithRecipe", instance.BuiltWithRecipe, ctx_);
                UI::EditorObjectReference("BlueprintProxy", instance.BlueprintProxy, ctx_);

                UI::EndEditorTable();
            }
            UI::PopEditorTableStyle();
        }
    }
    ImGui::PopID();
}
