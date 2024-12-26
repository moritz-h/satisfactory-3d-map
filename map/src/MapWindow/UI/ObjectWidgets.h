#pragma once

#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

#include "SatisfactorySave/GameTypes/FactoryGame/FGObjectReference.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Color.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Transform.h"

#include "CommonUI.h"
#include "Utils/GLMUtil.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap::UI {

    bool BeginEditorTable();
    void EndEditorTable();
    // Need extra Push/Pop for style, because style must be set before BeginTable() and EndEditorTable() must be only
    // called if BeginEditorTable() returns true, but style reset is always required.
    void PushEditorTableStyle();
    void PopEditorTableStyle();

    bool TreeNodeSmall(const char* label, ImGuiTreeNodeFlags flags = 0);
    bool EditorTreeNode(const char* label, ImGuiTreeNodeFlags flags = 0);
    bool EditorTreeStartLeaf(const char* label, ImGuiTreeNodeFlags flags = 0);
    void EditorTreeEndLeaf();

    void ClassOrPathButton(const std::string& name, const EventContext& ctx = {});
    void EditorShowSelectable(const char* label, const std::string& name, const EventContext& ctx = {});
    void EditorShowText(const char* label, const char* text);

    bool EditorBool(const char* label, bool& v);
    bool EditorScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = nullptr,
        const void* p_step_fast = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = 0);

    bool EditorObjectReference(const char* label, s::FObjectReferenceDisc& r, const EventContext& ctx = {});

    bool EditorLinearColor(const char* label, s::FLinearColor& c);

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
            ImGui::SetItemTooltip(glm::to_string(glmCast(t.Rotation)).c_str());
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
