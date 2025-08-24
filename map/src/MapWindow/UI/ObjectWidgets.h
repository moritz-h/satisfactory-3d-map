#pragma once

#include <optional>
#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorItem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGDynamicStruct.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGInventoryComponent.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGObjectReference.h"
#include "SatisfactorySave/GameTypes/Properties/Base/Property.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyList.h"
#include "SatisfactorySave/GameTypes/UE/Core/Containers/Map.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Color.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/IntPoint.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/IntVector.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Quat.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Rotator.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Transform.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Vector.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Vector2D.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Vector4.h"
#include "SatisfactorySave/GameTypes/UE/Core/UObject/NameTypes.h"
#include "SatisfactorySave/GameTypes/UE/CoreUObject/UObject/SoftObjectPath.h"

#include "CommonUI.h"
#include "Utils/GLMUtil.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap::UI {

    // Property Table
    template<ImGuiIdType Id_T = int>
    bool BeginEditorTable(Id_T id = 0);
    void EndEditorTable();
    // Need extra Push/Pop for style, because style must be set before BeginTable() and EndEditorTable() must be only
    // called if BeginEditorTable() returns true, but style reset is always required.
    void PushEditorTableStyle();
    void PopEditorTableStyle();

    template<ImGuiIdType Id_T, typename Callable>
    void EditorSectionWrap(Id_T id, const char* label, Callable callable);

    // Tree Utils
    bool TreeNodeSmall(const char* label, ImGuiTreeNodeFlags flags = 0);
    bool TreeNodeSmallEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    bool EditorTreeNode(const char* label, ImGuiTreeNodeFlags flags = 0);
    bool EditorTreeStartLeaf(const char* label, ImGuiTreeNodeFlags flags = 0);
    void EditorTreeEndLeaf();

    // Container Utils
    template<typename T, typename Callable>
    void EditorList(const char* label, std::vector<T>& list, Callable itemHandler);
    template<typename Key_T, typename Value_T, typename Key_Callable, typename Value_Callable>
    void EditorMap(const char* label, s::TMap<Key_T, Value_T>& map, Key_Callable keyHandler,
        Value_Callable valueHandler);
    template<typename T, typename Callable>
    void EditorOptional(const char* label, std::optional<T>& opt, Callable itemHandler);

    // Readonly Text/Data Widgets
    void ClassOrPathButton(const std::string& name, const EventContext& ctx = {});
    void EditorShowSelectable(const char* label, const std::string& name, const EventContext& ctx = {});
    // Disabled flag for value only. To also disable tree node wrap inside BeginDisabled()/EndDisabled().
    void EditorShowText(const char* label, const char* text, bool disabled = false);
    void EditorShowBinData(const char* label, const std::vector<char>& data, const EventContext& ctx = {});

    // Arithmetic Type Widgets
    bool EditorBool(const char* label, bool& v);
    bool EditorScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = nullptr,
        const void* p_step_fast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = 0);
    bool EditorString(const char* label, std::string& str);

    // UE Core Type Widgets
    bool EditorName(const char* label, s::FName& name);
    bool EditorObjectReference(const char* label, s::FObjectReferenceDisc& r, const EventContext& ctx = {});
    bool EditorSoftObjectPath(const char* label, s::FSoftObjectPath& p);

    // UE Math Type Widgets
    bool EditorVector(const char* label, s::FVector& v);
    bool EditorVector2D(const char* label, s::FVector2D& v);
    bool EditorVector4(const char* label, s::FVector4& v);
    bool EditorIntVector(const char* label, s::FIntVector& v);
    bool EditorIntPoint(const char* label, s::FIntPoint& p);
    bool EditorQuat(const char* label, s::FQuat& q);
    bool EditorRotator(const char* label, s::FRotator& r);
    template<typename T>
    bool EditorTransform(s::TTransform<T>& t);

    // Struct Widgets
    bool EditorColor(const char* label, s::FColor& c);
    bool EditorLinearColor(const char* label, s::FLinearColor& c);
    bool EditorInventoryItem(const char* label, s::FInventoryItem& i, const EventContext& ctx = {});
    bool EditorConveyorBeltItem(const char* label, s::FConveyorBeltItem& i, const EventContext& ctx = {});
    bool EditorDynamicStruct(const char* label, s::FFGDynamicStruct& s, const EventContext& ctx = {});

    // Property Widgets
    bool EditorProperty(s::Property& p, const EventContext& ctx = {});
    bool EditorPropertyList(const char* label, s::PropertyList& properties, const EventContext& ctx = {});

    template<ImGuiIdType Id_T>
    bool BeginEditorTable(Id_T id) {
        if (ImGui::BeginTable("##PropertyTable", 3, ImGuiTableFlags_Resizable)) {
            ImGui::PushID(id);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 1.0f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
            ImGui::TableSetupColumn("Button", ImGuiTableColumnFlags_WidthFixed, 15.0f);
            return true;
        }
        return false;
    }

    // Template implementations

    template<ImGuiIdType Id_T, typename Callable>
    void EditorSectionWrap(Id_T id, const char* label, Callable callable) {
        if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
            PushEditorTableStyle();
            if (BeginEditorTable(id)) {
                callable();
                EndEditorTable();
            }
            PopEditorTableStyle();
        }
    }

    template<typename T, typename Callable>
    void EditorList(const char* label, std::vector<T>& list, Callable itemHandler) {
        if (list.empty()) {
            EditorShowText(label, "List (Empty)");
        } else {
            const bool open = EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen);
            ImGui::TableNextColumn();
            ImGui::Text("List (Size: %zu)", list.size());
            if (open) {
                for (std::size_t i = 0; i < list.size(); i++) {
                    itemHandler(i, list[i]);
                }
                ImGui::TreePop();
            }
        }
    }

    template<typename Key_T, typename Value_T, typename Key_Callable, typename Value_Callable>
    void EditorMap(const char* label, s::TMap<Key_T, Value_T>& map, Key_Callable keyHandler,
        Value_Callable valueHandler) {
        if (map.empty()) {
            EditorShowText(label, "Map (Empty)");
        } else {
            const bool open = EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen);
            ImGui::TableNextColumn();
            ImGui::Text("Map (Size: %zu)", map.size());
            if (open) {
                for (std::size_t i = 0; i < map.size(); i++) {
                    if (EditorTreeNode(("#" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                        keyHandler(map.Keys[i]);
                        valueHandler(map.Values[i]);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
    }

    template<typename T, typename Callable>
    void EditorOptional(const char* label, std::optional<T>& opt, Callable itemHandler) {
        bool open = false;
        bool leaf = false;
        if (opt.has_value()) {
            open = EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen);
        } else {
            EditorTreeStartLeaf(label);
            leaf = true;
        }
        ImGui::TableNextColumn();
        ImGui::BeginDisabled();
        bool has_value = opt.has_value();
        ImGui::Checkbox("Optional", &has_value);
        ImGui::EndDisabled();
        if (leaf) {
            EditorTreeEndLeaf();
        }
        if (open) {
            if (opt.has_value()) {
                itemHandler(opt.value());
            }
            ImGui::TreePop();
        }
    }

    template<typename T>
    bool EditorTransform(s::TTransform<T>& t) {
        // For better UX we want to show euler angles in the UI with the full range of 0 to 360 degree on
        // each axis. But the mapping of rotation to euler angles is not unique. Therefore, we need to know
        // and edit the previous euler angle state and cannot map dynamically from quaternions to euler
        // angles in each frame.
        // TODO The current caching strategy will break as soon as anybody else updates the actor.
        bool changed = false;
        if (EditorTreeNode("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            static const s::TTransform<T>* cachedTransform = nullptr;
            static glm::vec3 posMeter = glmCast(t.Translation) / 100.0f;
            static glm::vec3 eulerAngels(0.0f);
            static glm::vec3 scale(0.0f);
            if (&t != cachedTransform) {
                cachedTransform = &t;
                posMeter = glmCast(t.Translation) / 100.0f;
                eulerAngels = glm::degrees(glm::eulerAngles(glmCast(t.Rotation)));
                while (eulerAngels.x < 0.0f) {
                    eulerAngels.x += 360.0f;
                }
                while (eulerAngels.x >= 360.0f) {
                    eulerAngels.x -= 360.0f;
                }
                while (eulerAngels.y < 0.0f) {
                    eulerAngels.y += 360.0f;
                }
                while (eulerAngels.y >= 360.0f) {
                    eulerAngels.y -= 360.0f;
                }
                while (eulerAngels.z < 0.0f) {
                    eulerAngels.z += 360.0f;
                }
                while (eulerAngels.z >= 360.0f) {
                    eulerAngels.z -= 360.0f;
                }
                scale = glmCast(t.Scale3D);
            }

            EditorTreeStartLeaf("Location");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::DragFloat3("##pos", glm::value_ptr(posMeter), 0.1f, 0.0f, 0.0f, "%.2f")) {
                changed = true;
                t.Translation = ueVectorCast<T>(posMeter * 100.0f);
            }
            EditorTreeEndLeaf();
            EditorTreeStartLeaf("Rotation");
            ImGui::SetItemTooltip("%s", glm::to_string(glmCast(t.Rotation)).c_str());
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::DragFloat3("##rot", glm::value_ptr(eulerAngels), 1.0f, 0.0f, 360.0f, "%.3f",
                    ImGuiSliderFlags_AlwaysClamp)) {
                changed = true;
                t.Rotation = ueQuatCast<T>(glm::quat{glm::radians(eulerAngels)});
            }
            EditorTreeEndLeaf();
            EditorTreeStartLeaf("Scale");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::DragFloat3("##scale", glm::value_ptr(scale), 0.1f)) {
                changed = true;
                t.Scale3D = ueVectorCast<T>(scale);
            }
            EditorTreeEndLeaf();
            ImGui::TreePop();
        }
        return changed;
    }
} // namespace Satisfactory3DMap::UI
