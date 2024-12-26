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
        if (saveObject->Object != nullptr) {
            renderGui(*saveObject->Object);
        }
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

void Satisfactory3DMap::UI::ObjectEditor::renderGui(s::UObject& obj) {
    UObjectEditor e(*this);
    e.dispatch(obj);
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::UObject& o) {
    EditorSectionWrap("UObject", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AActor& o) {
    // Display AActor section first to match serialization order.
    EditorSectionWrap("AActor", [&]() {
        // TODO
    });
    visit(static_cast<s::UObject&>(o));
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGBuildableConveyorBase& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGBuildableConveyorBase", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGConveyorChainActor& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGConveyorChainActor", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGBuildableWire& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGBuildableWire", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGCircuitSubsystem& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGCircuitSubsystem", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGLightweightBuildableSubsystem& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGLightweightBuildableSubsystem", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGGameMode& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGGameMode", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGGameState& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGGameState", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGPlayerState& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGPlayerState", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGVehicle& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGVehicle", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGRailroadVehicle& o) {
    visit(static_cast<s::AFGVehicle&>(o));
    EditorSectionWrap("AFGRailroadVehicle", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGDroneVehicle& o) {
    visit(static_cast<s::AFGVehicle&>(o));
    EditorSectionWrap("AFGDroneVehicle", [&]() {
        // TODO
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::UActorComponent& o) {
    visit(static_cast<s::UObject&>(o));
    EditorSectionWrap("UActorComponent", [&]() {
        // TODO
    });
}
