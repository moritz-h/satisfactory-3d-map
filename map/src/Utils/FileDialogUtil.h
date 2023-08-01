#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Satisfactory3DMap {
    std::optional<std::filesystem::path> openFile(std::string const& title, std::string const& default_path = "",
        std::vector<std::string> const& filters = {"All Files", "*"});

    std::optional<std::filesystem::path> saveFile(std::string const& title, std::string const& default_path = "",
        std::vector<std::string> const& filters = {"All Files", "*"});

    std::optional<std::filesystem::path> selectFolder(std::string const& title, std::string const& default_path = "");
} // namespace Satisfactory3DMap
