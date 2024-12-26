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

        EditorSectionWrap("ObjectBaseSaveHeader", [&]() {
            EditorShowSelectable("Class", saveObject->baseHeader().ClassName, ctx_);
            ImGui::BeginDisabled();
            EditorObjectReference("Reference", saveObject->baseHeader().Reference, ctx_);
            ImGui::EndDisabled();
        });
        if (proxy->isActor()) {
            EditorSectionWrap("ActorSaveHeader", [&]() {
                if (EditorTransform(saveObject->actorHeader().Transform)) {
                    ctx_.updateTransform(proxy);
                }
                ImGui::BeginDisabled();
                EditorBool("NeedTransform", saveObject->actorHeader().NeedTransform);
                EditorBool("WasPlacedInLevel", saveObject->actorHeader().WasPlacedInLevel);
                ImGui::EndDisabled();
            });
        } else {
            EditorSectionWrap("ObjectSaveHeader", [&]() {
                EditorShowSelectable("OuterPathName", saveObject->objectHeader().OuterPathName, ctx_);
            });
        }
        EditorSectionWrap("SaveObject", [&]() {
            ImGui::BeginDisabled();
            EditorScalar("SaveVersion", ImGuiDataType_S32, &saveObject->SaveVersion);
            EditorBool("ShouldMigrate", saveObject->ShouldMigrateObjectRefsToPersistent);
            ImGui::EndDisabled();
        });
        // TODO
    } else {
        EditorSectionWrap("LightweightBuildable", [&]() {
            auto& instance = proxy->getLightweightData();

            EditorShowSelectable("Class", proxy->className(), ctx_);
            EditorShowText("Path", proxy->pathName().c_str());
            if (EditorTransform(instance.Transform)) {
                ctx_.updateTransform(proxy);
            }
            EditorObjectReference("SwatchDesc", instance.CustomizationData.SwatchDesc, ctx_);
            EditorObjectReference("MaterialDesc", instance.CustomizationData.MaterialDesc, ctx_);
            EditorObjectReference("PatternDesc", instance.CustomizationData.PatternDesc, ctx_);
            EditorObjectReference("SkinDesc", instance.CustomizationData.SkinDesc, ctx_);
            if (EditorTreeNode("OverrideColorData", ImGuiTreeNodeFlags_DefaultOpen)) {
                EditorLinearColor("PrimaryColor", instance.CustomizationData.OverrideColorData.PrimaryColor);
                EditorLinearColor("SecondaryColor", instance.CustomizationData.OverrideColorData.SecondaryColor);
                EditorObjectReference("PaintFinish", instance.CustomizationData.OverrideColorData.PaintFinish, ctx_);
                ImGui::TreePop();
            }
            EditorScalar("PatternRotation", ImGuiDataType_U8, &instance.CustomizationData.PatternRotation);
            EditorObjectReference("BuiltWithRecipe", instance.BuiltWithRecipe, ctx_);
            EditorObjectReference("BlueprintProxy", instance.BlueprintProxy, ctx_);
        });
    }
    ImGui::PopID();
}
