#pragma once

#include <IconsFontAwesome6.h>
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

    template<typename T>
    void Transform(const s::TTransform<T>& t) {
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text(ICON_FA_CROSSHAIRS " Pos:    %s", glm::to_string(glmCast(t.Translation)).c_str());
            ImGui::Text(ICON_FA_ROTATE " Rot:    %s", glm::to_string(glmCast(t.Rotation)).c_str());
            ImGui::Text(ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER " Scale:  %s",
                glm::to_string(glmCast(t.Scale3D)).c_str());
            ImGui::TreePop();
        }
    }

    template<typename T>
    bool InputTransform(s::TTransform<T>& t) {
        // For better UX we want to show euler angles in the UI with the full range of 0 to 360 degree on
        // each axis. But the mapping of rotation to euler angles is not unique. Therefore, we need to know
        // and edit the previous euler angle state and cannot map dynamically from quaternions to euler
        // angles in each frame.
        // TODO The current caching strategy will break as soon as anybody else updates the actor.
        bool changed = false;
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
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

            if (ImGui::DragFloat3(ICON_FA_CROSSHAIRS " Pos", glm::value_ptr(posMeter), 0.1f, 0.0f, 0.0f, "%.2f")) {
                changed = true;
                t.Translation = ueVectorCast<T>(posMeter * 100.0f);
            }
            if (ImGui::DragFloat3(ICON_FA_ROTATE " Rot", glm::value_ptr(eulerAngels), 1.0f, 0.0f, 360.0f, "%.3f",
                    ImGuiSliderFlags_AlwaysClamp)) {
                changed = true;
                t.Rotation = ueQuatCast<T>(glm::quat{glm::radians(eulerAngels)});
            }
            if (ImGui::DragFloat3(ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER " Scale", glm::value_ptr(scale), 0.1f)) {
                changed = true;
                t.Scale3D = ueVectorCast<T>(scale);
            }
            ImGui::TreePop();
        }
        return changed;
    }

    void ClassOrPathButton(const std::string& name, const SelectionContext& ctx = {});
    void SelectableName(const char* label, const std::string& name, const SelectionContext& ctx = {});

    void ObjectReference(const char* label, const s::FObjectReferenceDisc& r, const SelectionContext& ctx = {});
    bool InputObjectReference(const char* label, s::FObjectReferenceDisc& r, const SelectionContext& ctx = {});

    void LinearColor(const char* label, const s::FLinearColor& c);
    bool InputLinearColor(const char* label, s::FLinearColor& c);
} // namespace Satisfactory3DMap::UI
