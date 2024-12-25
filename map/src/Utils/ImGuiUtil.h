#pragma once

#include <functional>
#include <string>

#include <imgui.h>

namespace Satisfactory3DMap::ImGuiUtil {
    static inline void PathLink(const std::string& pathName, const std::function<void(const std::string&)>& callback) {
        if (pathName.empty()) {
            // Probably ImGui::SameLine() was called before, so do something.
            ImGui::Text("");
        } else if (pathName[0] == '/') {
            // Absolute path names cannot be selected
            ImGui::Text("%s", pathName.c_str());
        } else {
            if (ImGui::SmallButton(pathName.c_str())) {
                callback(pathName);
            }
        }
    }
} // namespace Satisfactory3DMap::ImGuiUtil
