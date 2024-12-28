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
        EditorPropertyList("Properties", o.Properties, parent_.ctx_);
        EditorOptional("Guid", o.Guid, [&](auto& item) {
            EditorShowText("Value", "TODO FGuid!"); // TODO item
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AActor& o) {
    // Display AActor section first to match serialization order.
    EditorSectionWrap("AActor", [&]() {
        EditorObjectReference("Owner", o.Owner, parent_.ctx_);
        EditorList("Components", o.Components, [&](std::size_t idx, auto& item) {
            EditorObjectReference(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
        });
    });
    visit(static_cast<s::UObject&>(o));
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGBuildableConveyorBase& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGBuildableConveyorBase", [&]() {
        EditorList("mItems.Items", o.mItems.Items, [&](std::size_t idx, auto& item) {
            EditorConveyorBeltItem(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGConveyorChainActor& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGConveyorChainActor", [&]() {
        EditorObjectReference("mFirstConveyor", o.mFirstConveyor, parent_.ctx_);
        EditorObjectReference("mLastConveyor", o.mLastConveyor, parent_.ctx_);
        EditorList("mChainSplineSegments", o.mChainSplineSegments, [&](std::size_t idx, auto& item) {
            if (EditorTreeNode(("#" + std::to_string(idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                // FConveyorChainSplineSegment
                EditorObjectReference("ChainActor", item.ChainActor, parent_.ctx_);
                EditorObjectReference("ConveyorBase", item.ConveyorBase, parent_.ctx_);
                EditorList("SplinePointData", item.SplinePointData, [&](std::size_t idx2, auto& item2) {
                    if (EditorTreeNode(("#" + std::to_string(idx2)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                        // FSplinePointData
                        EditorVector("Location", item2.Location);
                        EditorVector("ArriveTangent", item2.ArriveTangent);
                        EditorVector("LeaveTangent", item2.LeaveTangent);
                        ImGui::TreePop();
                    }
                });
                EditorScalar("OffsetAtStart", ImGuiDataType_Float, &item.OffsetAtStart);
                EditorScalar("StartsAtLength", ImGuiDataType_Float, &item.StartsAtLength);
                EditorScalar("EndsAtLength", ImGuiDataType_Float, &item.EndsAtLength);
                EditorScalar("FirstItemIndex", ImGuiDataType_S32, &item.FirstItemIndex);
                EditorScalar("LastItemIndex", ImGuiDataType_S32, &item.LastItemIndex);
                EditorScalar("IndexInChainArray", ImGuiDataType_S32, &item.IndexInChainArray);
                ImGui::TreePop();
            }
        });
        EditorScalar("mTotalLength", ImGuiDataType_Float, &o.mTotalLength);
        EditorScalar("mNumItems", ImGuiDataType_S32, &o.mNumItems);
        EditorScalar("mLeadItemIndex", ImGuiDataType_S32, &o.mLeadItemIndex);
        EditorScalar("mTailItemIndex", ImGuiDataType_S32, &o.mTailItemIndex);
        EditorList("mConveyorChainItems", o.mConveyorChainItems, [&](std::size_t idx, auto& item) {
            EditorConveyorBeltItem(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGBuildableWire& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGBuildableWire", [&]() {
        EditorObjectReference("mConnections[0]", o.mConnections[0], parent_.ctx_);
        EditorObjectReference("mConnections[1]", o.mConnections[1], parent_.ctx_);
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGCircuitSubsystem& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGCircuitSubsystem", [&]() {
        EditorMap(
            "mCircuits", o.mCircuits,
            [&](auto& key) {
                EditorScalar("Key", ImGuiDataType_S32, &key);
            },
            [&](auto& value) {
                EditorObjectReference("Value", value, parent_.ctx_);
            });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGLightweightBuildableSubsystem& o) {
    visit(static_cast<s::AActor&>(o));
    if (ImGui::CollapsingHeader("AFGLightweightBuildableSubsystem", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextUnformatted("AFGLightweightBuildableSubsystem data is accessed with proxy objects.");
    }
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGGameMode& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGGameMode", [&]() {
        EditorList("rawPlayerStatePointers", o.rawPlayerStatePointers, [&](std::size_t idx, auto& item) {
            EditorObjectReference(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGGameState& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGGameState", [&]() {
        EditorList("rawPlayerStatePointers", o.rawPlayerStatePointers, [&](std::size_t idx, auto& item) {
            EditorObjectReference(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGPlayerState& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGPlayerState", [&]() {
        EditorShowText("Id", "TODO FUniqueNetIdRepl!"); // TODO: o.Id
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGVehicle& o) {
    visit(static_cast<s::AActor&>(o));
    EditorSectionWrap("AFGVehicle", [&]() {
        EditorList("mStoredPhysicsData", o.mStoredPhysicsData, [&](std::size_t idx, auto& item) {
            if (EditorTreeNode(("#" + std::to_string(idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                // FVehiclePhysicsData
                EditorName("BoneName", item.BoneName);
                if (EditorTreeNode("BodyState", ImGuiTreeNodeFlags_DefaultOpen)) {
                    // FRigidBodyState
                    EditorVector("Position", item.BodyState.Position);
                    EditorQuat("Quaternion", item.BodyState.Quaternion);
                    EditorVector("AngVel", item.BodyState.AngVel);
                    EditorVector("LinVel", item.BodyState.LinVel);
                    EditorScalar("Flags", ImGuiDataType_U8, &item.BodyState.Flags);
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGRailroadVehicle& o) {
    visit(static_cast<s::AFGVehicle&>(o));
    EditorSectionWrap("AFGRailroadVehicle", [&]() {
        EditorObjectReference("mCoupledVehicleFront", o.mCoupledVehicleFront, parent_.ctx_);
        EditorObjectReference("mCoupledVehicleBack", o.mCoupledVehicleBack, parent_.ctx_);
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::AFGDroneVehicle& o) {
    visit(static_cast<s::AFGVehicle&>(o));
    EditorSectionWrap("AFGDroneVehicle", [&]() {
        EditorOptional("mActiveAction", o.mActiveAction, [&](auto& item) {
            // FDroneAction
            EditorName("actionStructName", item.actionStructName);
            EditorPropertyList("action", item.action, parent_.ctx_);
        });
        EditorList("mActionQueue", o.mActionQueue, [&](std::size_t idx, auto& item) {
            if (EditorTreeNode(("#" + std::to_string(idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                // FDroneAction
                EditorName("actionStructName", item.actionStructName);
                EditorPropertyList("action", item.action, parent_.ctx_);
                ImGui::TreePop();
            }
        });
    });
}

void Satisfactory3DMap::UI::ObjectEditor::UObjectEditor::visit(s::UActorComponent& o) {
    visit(static_cast<s::UObject&>(o));
    if (ImGui::CollapsingHeader("UActorComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextUnformatted("UActorComponent has no data.");
    }
}
